
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define FAILURE (-1)
#define SUCCESS (0)
// 0:sin cosJ[u
// 1:{1,2,3,4}
#define INPUT_DATA 1
#define L 4
#define DATA_LENGTH 16
#define N 6

using namespace std;
int fr_multi(double *dest_r, double *dest_i, const double *src_r,const double *src_i, const double *filter_r, const double *filter_i,
	int sampleSize)
{
	for(int i=0 ; i < sampleSize ; i++)
	{
		dest_r[i] = src_r[i] * filter_r[i] + (src_i[i]*filter_i[i])*-1;
		dest_i[i] = src_r[i] * filter_i[i] + (src_i[i]*filter_r[i]);
	}	
	return SUCCESS;
}

int conv(double *dest_r, const double *src_r, const double *filter_r,
	int sampleSize)

{
	for(int i=0; i< sampleSize; i++)
	{
		dest_r[i] = 0;
		for(int j = 0; j < sampleSize ; j++)
		{
			int _j_i = (j - i) % sampleSize;
			dest_r[i] += src_r[i] * filter_r[_j_i];
		}
	}
	return SUCCESS;
}

int dft(double *dest_r, double *dest_i,
	const double *src_r, const double *src_i, 
	int sampleSize, bool inverse)
{
	double sum_re, sum_im, pi, arg, freqSample;
	int sample;
	int freq;

	if(dest_r == NULL || dest_i == NULL ||
		src_r == NULL || src_i == NULL || sampleSize <= 1){
			return FAILURE;
	}

	// initializing a destination array
	for( sample=0; sample < sampleSize; sample++)
	{
		dest_r[sample] = 0.0;
		dest_i[sample] = 0.0;
	}

	pi = (inverse == false)? M_PI: -M_PI;

	// main loop
	for( freq=0; freq<sampleSize; freq++)
	{
		sum_re = 0.0;
		sum_im = 0.0;
		arg = ((double)freq/(double)sampleSize) * (2*pi);
		for( sample=0; sample<sampleSize; sample++)
		{
			freqSample = sample * arg;

			sum_re += src_r[sample] * cos( freqSample )
				+ src_i[sample] * sin( freqSample );
			sum_im += src_i[sample] * cos( freqSample )
				- src_r[sample] * sin( freqSample );
		}
		if(inverse){
			dest_r[freq] = sum_re;
			dest_i[freq] = sum_im;
		}else{
			dest_r[freq] = sum_re/(double)sampleSize;
			dest_i[freq] = sum_im/(double)sampleSize;
		}
	}

	return SUCCESS;
}

int saveArrayAsText1(void *array_src, int arraysize,
	char *const filename)
{
	int i;
	FILE *fp = NULL;

	if(array_src == NULL || arraysize==0)
	{ return FAILURE; }

	if((fp=fopen(filename, "w")) == NULL)
	{ return FAILURE; }

	for(i=0; i < arraysize; i++)
	{
		fprintf(fp, "%f\n", ((double*)array_src)[i]);
	}
	fclose(fp);

	return SUCCESS;
}

int main()
{
	int status, datasize;
	int i;
	datasize = N;
#if 0
	double ti_src_All[] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
	double ti_dst_All[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#else
	double *ti_src_All;
	double *ti_dst_All;
	ti_src_All = (double *)malloc(DATA_LENGTH * sizeof(double));
	ti_dst_All = (double *)malloc(DATA_LENGTH * sizeof(double));
	for(int i = 0; i< DATA_LENGTH; i++)
	{
		ti_src_All[i] = i % 4;
		ti_dst_All[i] = 0;
	}

#endif
	//double ti_src_r[] = {0,0,0,0,0,0};
	//double ti_src_i[] = {0,0,0,0,0,0};
	//double fr_src_r[] = {0,0,0,0,0,0};
	//double fr_src_i[] = {0,0,0,0,0,0};
	double fr_multi_src_r[] = {0,0,0,0,0,0};
	double fr_multi_src_i[] = {0,0,0,0,0,0};
	double ti_multi_dst_r[] = {0,0,0,0,0,0};
	double ti_multi_dst_i[] = {0,0,0,0,0,0};
	double ti_conv_dst_r[] = {0,0,0,0,0,0};
	double ti_conv_dst_i[] = {0,0,0,0,0,0};

	double filter_r[] = {0,1,0,0,0,0};
	double filter_i[] = {0,0,0,0,0,0};
	double fr_filter_r[]={0,0,0,0,0,0};
	double fr_filter_i[]={0,0,0,0,0,0};
	//double *im_r, *im_i, *dest_r, *dest_i;
	
	status = saveArrayAsText1(ti_src_All, datasize,
		"ti_src_All.txt");
	if(status == FAILURE)
	{ fprintf(stderr,"file write failed.\n");
	return FAILURE;}
	for(int idx = 0;idx < DATA_LENGTH; idx+=L)
	{
		/* æÔ²ÆÌ[v */
		double ti_src_r[N] = {0,0,0,0,0,0};
		double ti_src_i[N] = {0,0,0,0,0,0};
		double fr_src_r[N] = {0,0,0,0,0,0};
		double fr_src_i[N] = {0,0,0,0,0,0};
		cout << idx << " start" << endl;
		for(int i = 0; i < L;i++)
		{
			if(idx + i >= DATA_LENGTH)continue;
			ti_src_r[i] = ti_src_All[idx + i];

		}

		//dft

		status = dft(fr_src_r, fr_src_i, ti_src_r, ti_src_i,
			datasize, false);
		dft( fr_filter_r, fr_filter_i, filter_r, filter_i,
			datasize, false);
		if(status == FAILURE)
		{ return FAILURE;}
		// ôÝÝiügÌæÅÏj
		fr_multi(fr_multi_src_r, fr_multi_src_i, fr_src_r , fr_src_i , fr_filter_r, fr_filter_i, datasize);

		// ÊÌi[
		status = saveArrayAsText1(fr_multi_src_r, DATA_LENGTH, 
			"fr_multi_src_r.txt");
		//inverse dft
		status = dft(ti_multi_dst_r, ti_multi_dst_i, fr_multi_src_r, fr_multi_src_i,
			datasize, true);
		if(status == FAILURE)
		{ fprintf(stderr,"inverse dft failed.\n");
		return FAILURE;}
		for(int i=0;i<N;i++)
		{
			if(idx + i >= DATA_LENGTH )continue;
			ti_dst_All[idx+i] += ti_multi_dst_r[i];
		}
		cout << idx << " idx"<< endl;
	}
	

	status = saveArrayAsText1(ti_dst_All, DATA_LENGTH, 
		"idft_r.txt");
	if(status == FAILURE)
	{ fprintf(stderr,"saving file failed.\n");
	return FAILURE;}
	printf("done!\n");

	return SUCCESS;
}