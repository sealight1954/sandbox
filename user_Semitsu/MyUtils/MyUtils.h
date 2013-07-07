#include <windows.h>
#include <direct.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <time.h> // getTempTime
//#include <cv.h>
//#include <highgui.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
using namespace cv;

#define POW(x) ((x)*(x))

// _temp_time�Ɍ��ݎ������i�[
// �O��_temp_time[256]���`���Ă���
void getTempTime(char* _temp_time){
	//----���ݎ���temp_time
	time_t timer;
	time(&timer);/* ���݂̎������擾 */
	struct tm *local;
	
	local =localtime(&timer);/* �n�����̍\���̂ɕϊ� */
	//char temp_time[256];
	sprintf(_temp_time,"%4d%02d%02d%02d%02d%02d"
		,local->tm_year + 1900,/* �N */
		local->tm_mon+1,/*����0����n�܂�̂�1���{*/
		local->tm_mday,/* �� */
		local->tm_hour,/* �� */
		local->tm_min,/* �� */
		local->tm_sec);/* �b */
	//std::cout<< "time: " << temp_time<<std::endl;
	//return temp_time;
}

bool getExistFile(const char* path){
	FILE *fp;
	if ( (fp = fopen(path,"r")) != NULL ){
		fclose(fp);
		return true;
	}
	else{
		return false;
	}
}

bool getExistFolder(const char* path){
	FILE *fp;
	std::stringstream ss;
	ss.str("");
	ss<< path << "nul";
	if ( (fp = fopen(ss.str().c_str(),"r")) != NULL ){
		fclose(fp);
		return true;
	}
	else{
		return false;
	}
}



int checkMkdir(const char* path){
	if(!(getExistFolder(path))){
		if(_mkdir(path)==-1){
			cerr<<path<<"make dir error"<<endl;
			return -1;
		}else{
			return 0;
		}
	}
	//�t�H���_������
	return 1;
}
void splitPath(const char* path,std::string &_folderPath,std::string &_fname,std::string &_ext){
	char drive[_MAX_DRIVE];	// �h���C�u��
	char dir[_MAX_DIR];		// �f�B���N�g����/�܂�
	char fname[_MAX_FNAME];	// �t�@�C����
	char ext[_MAX_EXT];		// �g���q
	_splitpath(path, drive, dir, fname, ext );
	std::stringstream oss;
	oss.str("");
	oss<<drive<<dir;
	_folderPath=std::string(oss.str());
	_fname=std::string(fname);
	_ext=std::string(ext);
}

template<typename T>
void load_mat(const char* filename,cv::Mat& data_mat){
	int rows=data_mat.rows;
	int cols=data_mat.cols;
	{ // �T���v���̓ǂݍ���
		std::ifstream ifs(filename);
		//std::vector<float> data_;
		for (int i = 0; i < rows; i++) {
			for(int j=0;j<cols;j++){
				T temp;
				ifs >> temp;
				data_mat.at<T>(i,j)=temp;
			}
		}
	}
	//for(int row=0;row<data_mat.rows;row++){
	//for(int col=0;col<data_mat.cols;col++){
	//	cout << data_mat.at<double>(row,col);
	//}}
}
template<typename T>
void load_vec(const char* filename,int num,int dim, std::vector<std::vector<T>>& vec){
	std::ifstream ifs(filename);
	for(int i=0;i<num;i++){
		std::vector<T> temp_vec;
		for( int j=0;j<dim;j++){
			T temp;
			ifs >> temp;
			temp_vec.push_back(temp);
		}
		vec.push_back(temp_vec);
	}
}


template<typename T>
void save_mat(const cv::Mat &mat,const std::string &filename){
	std::ofstream fout(filename.c_str());
	for(int i=0;i<mat.rows;i++){
		for(int j=0;j<mat.cols;j++){
			fout << fixed<< mat.at<T>(i,j);
			if(j!=mat.cols-1)fout << " " ;
		}
		fout << std::endl;
	}
	fout.close();
}

int load_mask(const std::string filename,cv::Mat &dst,const int w,const int h){
	cv::Mat bgs,resize_bgs,th_bgs;
	bgs=cv::imread(filename,0);
	cv::resize(bgs,resize_bgs,cv::Size(w,h));
	cv::threshold(resize_bgs,th_bgs,20,255,CV_MINMAX);
	th_bgs.copyTo(dst);
	return cv::countNonZero(th_bgs);
}

const std::string MkdirDate(const char *path,const char *cam, const char* yyyy,const char* mm,const char* dd,const char* hh){
	stringstream ss;
	ss.str("");
	ss<< path;
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << "cam" << cam << "\\"; // cam***/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << yyyy << "\\";// yyyy/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << mm << "\\";// mm/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << dd << "\\";// dd/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << hh << "\\";// hh/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	std::cout << ss.str() <<std::endl;
	return ss.str();
}


const std::string MkdirDateSlash(const char *path,const char *cam, const char* yyyy,const char* mm,const char* dd,const char* hh){
	std::stringstream ss;
	ss.str("");
	ss<< path;
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << "cam" << cam << "/"; // cam***/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << yyyy << "/";// yyyy/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << mm << "/";// mm/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << dd << "/";// dd/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	ss << hh << "/";// hh/
	if(!(getExistFolder(ss.str().c_str())))_mkdir(ss.str().c_str());//�t�H���_�̑��݂��m�F���A�Ȃ��Ȃ�t�H���_���쐬
	std::cout << ss.str() <<std::endl;
	return ss.str();
}

// 1�񂲂Ƃɕ��U�P�A���ςO�ɂȂ�悤�ɐ��K��
//template<typename T>
void normalizeAsCol(cv::Mat& _data,cv::Mat& _means,cv::Mat& _stddevs){
	// ���K��
	int col=_data.cols;//�����ʂ̎���
	int row=_data.rows;//�T���v����

	cv::Mat dst_data(row,col,CV_64F);
	//cv::Mat dst_data(row,col,_data.depth());
	// �W���΍��A���ς����߂Đ��K��(1�s��vector)
	//#pragma omp parallel for
	for(int i=0;i<col;i++){
		cv::Scalar mean;
		cv::Scalar stddev;
		cv::Mat temp=_data.col(i);
		cv::meanStdDev(temp,mean,stddev);
		_means.at<double>(0,i)=mean[0];
		_stddevs.at<double>(0,i)=stddev[0];
		for(int j=0;j<row;j++){
			dst_data.at<double>(j,i)=(stddev[0]==0)?0.0:(_data.at<double>(j,i)-mean[0])/stddev[0];
		}
	}
	_data=dst_data;
}

// ���Ɍv�Z���ꂽ�l���g���Đ��K��
void normalizeAsCol2(cv::Mat& _data,const cv::Mat& means,const cv::Mat& stddevs){
	// ���K��
	int col=_data.cols;//�����ʂ̎���
	int row=_data.rows;//�T���v����
	//cv::Mat dst_data(row,col,_data.depth());
	// �W���΍��A���ς����߂Đ��K��(1�s��vector)
	//for(int i=0;i<col;i++){
	//	double stddev=stddevs.at<double>(0,i);
	//	double mean=means.at<double>(0,i);
	//	for(int j=0;j<row;j++){
	//		dst_data.at<double>(j,i)=(stddev==0)?0.0:((_data.at<double>(j,i)-mean)/stddev);
	//	}
	//}
	const double *stddevs_ptr=stddevs.ptr<double>(0);
	const double *means_ptr=means.ptr<double>(0);
#pragma omp parallel for
	for(int j=0;j<row;j++){
		//double *dst_data_ptr=dst_data.ptr<double>(j);
		double *_data_ptr=_data.ptr<double>(j);
		for(int i=0;i<col;i++){
			_data_ptr[i]=(stddevs_ptr[i]==0)?0.0:((_data_ptr[i] - means_ptr[i])/stddevs_ptr[i]);
		}
	}
	//_data=dst_data;
}
template<typename T>
double euclidDistance(const Mat &mat1,const Mat &mat2){
	const T *mat_ptr1=mat1.ptr<T>(0);
	const T *mat_ptr2=mat2.ptr<T>(0);
	double sum=0.0;
	for(int i=0;i<mat1.cols;i++){
		sum += POW(mat_ptr1[i]-mat_ptr2[i]);
	}
	return sqrt(sum);
}

Mat src2GrayResize32Mat(const Mat &src,const int& w,const int& h){
	// �T�C�Y�ύX�p
	Mat resize_img;
	// �O���[�X�P�[�����ύX�p
	Mat gray_img;
	// 32bit���������^�̍s��ɕϊ�
	Mat c32_mat(h,w,CV_32F);
	cvtColor(src,gray_img,CV_RGB2GRAY);
	resize(gray_img,resize_img,Size(w,h));
	return resize_img.clone();
}
Mat src2Gray(const Mat &src){
	Mat gray_img;
	cvtColor(src,gray_img,CV_RGB2GRAY);
	return gray_img.clone();
}
Mat src2Resize(const Mat &src,const int &w,const int &h){
	Mat resize_mat;
	resize(src,resize_mat,Size(w,h));
	return resize_mat.clone();
}
Mat src232F(const Mat &src){
	Mat c32_mat;
	src.convertTo(c32_mat,CV_32F);
	return c32_mat.clone();
}
Mat src2Cvt(const Mat &src,int type){
	Mat cvtMat;
	src.convertTo(cvtMat,type);
	return cvtMat.clone();
}
Mat src2Threshold(const Mat &src,double th,double maxval,int type){
	Mat th_mat;
	threshold(src,th_mat,th,maxval,type);
	return th_mat.clone();
}

/*!
* @brief source:�u���Ώ� pattern:�����p�^�[�� placement:�u���p�^�[��
* @return �u����̕�����
* */
string replace_all(	const string & source,	const string & pattern,	const string & placement)
{
	string result;
	string::size_type pos_before = 0;
	string::size_type pos = 0;
	string::size_type len = pattern.size();
	while ((pos = source.find(pattern, pos)) != string::npos) {
		result.append(source, pos_before, pos - pos_before);
		result.append(placement);
		pos += len;
		pos_before = pos;
	}
	result.append(source, pos_before, source.size() - pos_before);
	return result;
}
	/*!
	 * @brief �摜���̃L�[�|�C���g�̉�f�l����ׂ�samples*3�̃��X�g�����
	 * @return samples*3�̃��X�g
	 * */
Mat loadValMat(Mat img,vector<KeyPoint> keypointLst){
	Mat dst=Mat(keypointLst.size(),3,CV_32F);
	for(int i=0;i<keypointLst.size();i++){
		KeyPoint key = keypointLst[i];
		dst.at<float>(i,0)=(float)img.at<Vec3b>(key.pt)[0];
		dst.at<float>(i,1)=(float)img.at<Vec3b>(key.pt)[1];
		dst.at<float>(i,2)=(float)img.at<Vec3b>(key.pt)[2];
	}
	return dst.clone();
}