
#include <iostream>
#include <fstream>
#include <list>
#include <opencv.hpp>
using namespace cv;
using namespace std;
namespace IoUtils{
	int getRowsOfFile(string filename);
	int getColsOfFile(string filename,string sep);
	//Rect getRectMask(const Mat &maskImg);
	int loadImgLst(const string filename, vector<Mat> &dst);
	int loadFileLst(const string fnameLst,vector<string> &dst);
	// cv::FileStorage‚ğg‚Á‚½“Ç‚İ‘‚«
	Mat cvLoadMat(const string &path,const string &varName);
	void cvSaveMat(const string &path,const string &varName,const Mat &data);
	template<typename T>
	void coutMat(const Mat &mat){
		for(int i=0;i<mat.rows;i++)
		{
			for(int j=0;j<mat.cols;j++)
			{
				cerr<</*fixed<<*/mat.at<T>(i,j)<<" ";
			}
			cerr<<endl;
		}
	}
	template<typename T>
	void saveVec(vector<T> vec,string filename){
		std::ofstream fout(filename.c_str());
		for(int i=0;i<vec.size();i++){
			fout<<vec[i];
			fout << std::endl;
		}
		fout.close();
	}
	void saveKeypoints(const string& path,const vector<KeyPoint> &keypointLst,const Mat &descriptorLst);
	void showHist(const Mat &hist);
}