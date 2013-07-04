
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define FAILURE (-1)
#define SUCCESS (0)
// 0:sin cosÉJÅ[Éu
// 1:{1,2,3,4}
#define INPUT_DATA 1

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
#if INPUT_DATA == 0
	double *im_r, *im_i, *dest_r, *dest_i;
	datasize = 256;

	im_r = (double*)malloc(datasize*sizeof(double));
	im_i = (double*)malloc(datasize*sizeof(double));
	dest_r = (double*)malloc(datasize*sizeof(double));
	dest_i = (double*)malloc(datasize*sizeof(double));

	//initialize
	for(i=0; i< datasize; i++)
	{
		double point = (double)i;
		im_r[i] = sin(point)+3*sin(2*point)
			+5*sin(5*point)+3*cos(point);
		im_i[i] = 0;
	}
#elif INPUT_DATA == 1
	datasize = 4;
	double ti_src_r[] = {1,2,3,4};
	double ti_src_i[] = {0,0,0,0};
	double fr_src_r[] = {0,0,0,0};
	double fr_src_i[] = {0,0,0,0};
	double fr_multi_src_r[] = {0,0,0,0};
	double fr_multi_src_i[] = {0,0,0,0};
	double ti_multi_dst_r[] = {0,0,0,0};
	double ti_multi_dst_i[] = {0,0,0,0};
	double ti_conv_dst_r[] = {0,0,0,0};
	double ti_conv_dst_i[] = {0,0,0,0};

	double filter_r[]= {0,1,0,0};
	double filter_i[]={0,0,0,0};
	double fr_filter_r[]={0,0,0,0};
	double fr_filter_i[]={0,0,0,0};
	//double *im_r, *im_i, *dest_r, *dest_i;
#endif
#if INPUT_DATA == 0
	status = saveArrayAsText1(im_r, datasize,
		"src_r.txt");
#elif INPUT_DATA == 1
	status = saveArrayAsText1(ti_src_r, datasize,
		"ti_src_r.txt");
#endif
	if(status == FAILURE)
	{ fprintf(stderr,"file write failed.\n");
	return FAILURE;}

	//dft
#if INPUT_DATA == 0
	status = dft(dest_r, dest_i, im_r, im_i,
		datasize, false);
#elif INPUT_DATA == 1
	status = dft(fr_src_r, fr_src_i, ti_src_r, ti_src_i,
		datasize, false);
	dft( fr_filter_r, fr_filter_i, filter_r, filter_i,
		datasize, false);
#endif
	if(status == FAILURE)
	{ return FAILURE;}
#if INPUT_DATA == 1
	// èÙÇ›çûÇ›Åié¸îgêîóÃàÊÇ≈êœÅj
	fr_multi(fr_multi_src_r, fr_multi_src_i, fr_src_r , fr_src_i , fr_filter_r, fr_filter_i, datasize);
	//fr_multi(fr_multi_src_i,  fr_src_i,fr_filter_i, datasize);
//for(int i =0 ; i < datasize; i++)
//{
//	fr_multi_src_i[i] = fr_src_i[i];
//	fr_multi_src_r[i]  = fr_src_r[i];
//}
#endif

	// åãâ ÇÃäiî[
#if INPUT_DATA == 0
	status = saveArrayAsText1(dest_r, datasize, 
		"dft_r.txt");
	if(status == FAILURE)
	{ fprintf(stderr,"saving file failed.\n");
	return FAILURE;}

	status = saveArrayAsText1(dest_i, datasize,
		"dft_i.txt");
	if(status == FAILURE)
	{ fprintf(stderr,"saving file failed.\n");
	return FAILURE;}
#elif INPUT_DATA == 1
	status = saveArrayAsText1(fr_multi_src_r, datasize, 

		"fr_multi_src_r.txt");
#endif
	

	//inverse dft
#if INPUT_DATA == 0
	status = dft(im_r,im_i,dest_r,dest_i,datasize,true);
#elif INPUT_DATA == 1
	status = dft(ti_multi_dst_r, ti_multi_dst_i, fr_multi_src_r, fr_multi_src_i,
		datasize, true);
#endif
	if(status == FAILURE)
	{ fprintf(stderr,"inverse dft failed.\n");
	return FAILURE;}
#if INPUT_DATA == 0 
	status = saveArrayAsText1(im_r, datasize, 
		"idft_r.txt");
#elif INPUT_DATA == 1
	status = saveArrayAsText1(ti_multi_dst_r, datasize, 
		"idft_r.txt");

#endif
	if(status == FAILURE)
	{ fprintf(stderr,"saving file failed.\n");
	return FAILURE;}
#if INPUT_DATA == 0
	free(im_r);
	free(im_i);
	free(dest_r);
	free(dest_i);
#endif
	printf("done!\n");

	return SUCCESS;
}