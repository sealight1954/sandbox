#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
using namespace cv;
#define POW(x) ((x)*(x))

/*!
* @return mat1とmat2の相関
* */

double correl_distance(const Mat& mat1,const Mat& mat2){
	if(mat1.cols!=mat2.cols){
		cerr<<"wrong cols:mat1 and mat2"<<endl;
		return -9999999;
	}
	const double* ptr1=mat1.ptr<double>(0);
	const double* ptr2=mat2.ptr<double>(0);
	Scalar _mean1=mean(mat1);
	double mean1=_mean1[0];
	Scalar _mean2=mean(mat2);
	double mean2=_mean2[0];
	double sum1=0.0;
	double sum2=0.0;
	double sum_=0.0;
	for(int i=0;i<mat1.cols;i++){
		double val1=ptr1[i]-mean1;
		double val2=ptr2[i]-mean2;
		sum1+=POW(val1);
		sum2+=POW(val2);
		sum_+=val1*val2;
	}
	double result=sum_/sqrt(sum1*sum2);
	return result;
}
/*!
* @brief RGB3chのヒストグラム（1行にまとめてある）のそれぞれの相関の和を返す
* @return
* */
double rgbCorrel(const Mat& mat1,const Mat& mat2){
	if(mat1.cols!=mat2.cols){
		cerr<<"wrong cols:mat1 and mat2"<<endl;
		return -9999999;
	}
	if(mat1.cols%3!=0)return -8888;
	int bins=mat1.cols/3;
	double val[3];
	for(int i=0;i<3;i++){
		Mat temp1=Mat(mat1,Rect(i*bins,0,bins,1));
		Mat temp2=Mat(mat2,Rect(i*bins,0,bins,1));
		val[i]=correl_distance(temp1,temp2);
	}
	return val[0]+val[1]+val[2];
}


double calc_hist_intersection(const Mat& mat1,const Mat& mat2){
	double total = 0;
	for(int i=0;i<mat1.cols;i++){
		cerr<<mat1.at<double>(i)<<" "<<mat2.at<double>(i)<<endl;
		total += min(mat1.at<double>(i), mat2.at<double>(i));
	}
	return total / sum(mat1)[0];
}