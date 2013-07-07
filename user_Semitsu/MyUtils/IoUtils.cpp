#include "IoUtils.h"
using namespace cv;
using namespace std;

namespace IoUtils{
	/*!
	* @brief ファイルの行数を数える
	* */
	int getRowsOfFile(string filename){
		ifstream f_temp(filename.c_str());
		if(!f_temp){
			cerr<<"Error in getRowsOfFile() cannot open file :"<<filename<<endl;
			return -1;
		}
		string temp;
		int samples=0;
		while(getline(f_temp,temp)){samples=samples+1;}
		return samples;
	}

	/*!
	* @brief ファイルの列数を数える
	* */
	int getColsOfFile(string filename,string sep){
		ifstream fin(filename.c_str());
		if(!fin){
			cerr<<"Error in getColsOfFile() cannot open file :"<<filename<<endl;
			return -1;
		}
		string str;
		if(!getline(fin,str))cerr<<"failed loading"<<endl;
		/**
		* split関数
		* @param string str 分割したい文字列
		* @param string delim デリミタ
		* @return list<string> 分割された文字列
		*/

		list<string> result;
		int cutAt;
		while( (cutAt = str.find_first_of(sep)) != str.npos )
		{
			if(cutAt > 0){
				result.push_back(str.substr(0, cutAt));
			}
			str = str.substr(cutAt + 1);
		}
		if(str.length() > 0)
		{
			result.push_back(str);
		}
		return result.size();
	}

	/*!
	* @brief ファイルリストを引数にして画像リストを読み込む
	* @return ファイル数
	* */
	int loadImgLst(const string filename, vector<Mat> &dst){
		ifstream fin(filename.c_str());
		if(!fin){
			cerr<<"Error in loadImgLst() cannot open file :"<<filename<<endl;
			return -1;
		}
		string imgname;
		int samples=0;
		while(fin>>imgname)
		{
			Mat temp=imread(imgname);
			dst.push_back(temp.clone());
			samples++;
		}
		return samples;
	}
	/*!
	* @brief ファイルリストを引数にしてファイルリストのvectorを生成
	* @return ファイル数
	* */
	int loadFileLst(const string fnameLst,vector<string> &dst){
		ifstream fin(fnameLst.c_str());
		if(!fin){
			cerr<<"Error in loadFileLst() cannot open file :"<<fnameLst<<endl;
			return -1;
		}
		string imgname;
		int samples=0;
		while(fin>>imgname)
		{
			dst.push_back(imgname);
			samples++;
		}
		return samples;
	}

	/*!
	* @brief ファイルストレージからMatを読込
	* */
	Mat cvLoadMat(const string &path,const string &varName){
		FileStorage fs(path.c_str(),FileStorage::READ);
		if(!(fs.isOpened())){
			cerr<<"Error in cvLoadMat() cannot open file"<<endl;

		}
		Mat temp;
		read(fs[varName.c_str()],temp);
		return temp.clone();
	}
	/*!
	* @brief ファイルストレージにMatを保存
	* */
	void cvSaveMat(const string &path,const string &varName,const Mat &data){
		FileStorage fs(path,FileStorage::WRITE);
			if(!(fs.isOpened())){
			cerr<<"Error in cvSaveMat() cannot open file"<<endl;

		}
		write(fs,varName,data);
	}


	template<typename T>
	void cvSaveVal(const string &path,const string &varName,const T &data){
		FileStorage fs(path,FileStorage::WRITE);
		if(!fs.isOpened){
			cerr<<"Error in cvSaveVal() cannot open file"<<endl;

		}
		write(fs,varName,data);

	}

	/*!
	* @brief keypointとそれに対応するdescriptorを読み込む
	* @return ファイル数
	* */
	void saveKeypoints(const string& path,const vector<KeyPoint> &keypointLst,const Mat& descriptorLst){
		ofstream fout(path.c_str());
		fout << keypointLst.size() << " " << descriptorLst.cols<<endl;
		{
			vector<KeyPoint>::const_iterator key_it;
			int i;
			for(i=0,key_it=keypointLst.begin();key_it!=keypointLst.end();key_it++,i++){
				fout << key_it->pt.x << " " << key_it->pt.y << " "<<key_it->size <<" "<<key_it->angle<<" ";
				const float* des_ptr=descriptorLst.ptr<float>(i);
				for(int j=0;j<descriptorLst.cols;j++){
					fout << des_ptr[j]<<" ";
				}//for j:128次元
				fout<<endl;
			}//for i:キーポイント数
		}
	}


	// hist from calcHist 
	/*!
	* @brief calcHistで求めたようなMatを描画
	* */
	void showHist(const Mat &hist){
		int hist_w=200;
		int hist_h=200;
		int bin_w=hist_w/hist.rows;
		double maxval=-10000;
		for(int i=0;i<hist.rows;i++){
			if(maxval<hist.at<int>(i,0))maxval=hist.at<int>(i,0);
		}
		Mat hist_img=Mat::zeros(hist_w,hist_h,CV_8U);
		for(int i=0;i<hist.rows;i++){
			rectangle(hist_img,Point(i*bin_w,hist_img.rows),Point((i+1)*bin_w,hist_img.rows - hist.at<int>(i,0)* hist_h /  maxval),Scalar((0xaa<<i*8)&0x0000ff,(0xaa<<i*8)&0x00ff00,(0xaa<<i*8)&0xff0000, 0));

		}
		imshow("hist",hist_img);
		waitKey();
	}

}