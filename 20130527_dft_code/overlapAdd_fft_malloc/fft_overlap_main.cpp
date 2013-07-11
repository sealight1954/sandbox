#include "fft.hpp"
#include "dftUtils.hpp"
#include "Utils.hpp"
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "MyUtils.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// 0:sin cosカーブ
// 1:{1,2,3,4}
#define INPUT_DATA 1
//overlapAddの一区間
#define L 10
//全データ長
#define DATA_LENGTH 100
//DFTサイズ
#define N 16
using namespace cv;
using namespace std;



int main(const int argc, const char *argv[])
{

	int status, datasize;
	int i;
	datasize = N;

	string filepath = "C:\\Users\\takayuki\\Pictures\\CELTICS.jpg";
	cv::Mat srcImg = imread(filepath,0);
	if(srcImg.empty())
	{
		cout << "file open failed:" << filepath <<  endl;
		exit(EXIT_FAILURE);
	}

	/* リサイズ */
	Mat resizeImg = src2Resize(srcImg, DATA_LENGTH, DATA_LENGTH);

	Mat filterImg = Mat::zeros(Size(DATA_LENGTH, DATA_LENGTH),CV_8UC1);
	filterImg.at<unsigned char>(DATA_LENGTH/2 + 1, DATA_LENGTH / 2 + 1) = 1;

	double** ptrptr_resizeImg;
	//for(int i = 0; i < DATA_LENGTH; i++)
	ptrptr_resizeImg = (double **)malloc(sizeof(double*) * DATA_LENGTH);
	fft::Fft fft(argc,argv);
	fft::FftArray arr(4);

	/*横方向DFT*/

	/* L区切りで区間ごとにDFT */
	for(int idx = 0;idx < DATA_LENGTH; idx+=L)
	{
		/* 区間ごとのループ */

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
		// 畳み込み（周波数領域で積）
		fr_multi(fr_multi_src_r, fr_multi_src_i, fr_src_r , fr_src_i , fr_filter_r, fr_filter_i, datasize);

		// 結果の格納
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