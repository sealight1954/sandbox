
#include <iostream>
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <boost/random.hpp>
#include <boost/format.hpp>
#include <opencv2/legacy/legacy.hpp>//OpenCV2.4.5以降では少なくとも必要
#include <opencv2/gpu/gpu.hpp>
#include <algorithm>

#include <sstream>
//#include "KAZE.h"
//#include "config.h"
//#include "utils.h"
#include "Utils.h"
using namespace cv;
using namespace std;
#define _DESCRIPTOR_MODE      0 
#define _DESCRIPTOR_SIFT      0
#define _DESCRIPTOR_SURF      1
#define _DESCRIPTOR_ORB       2
#define _DESCRIPTOR_CALONDER  3
#define _DESCRIPTOR_BRIEF     4
#define _DESCRIPTOR_HOG       5
#define _DESCRIPTOR_GPU_HOG   6
#define _IMSHOW_DEBUG
static bool isDMatchZeroDistance(const DMatch &m)
{
	//return m.distance == 0.0f;
	return m.distance < FLT_EPSILON;

}
static bool isDMatch1BiggerThan2(const DMatch &c1, const DMatch &c2)
{
	return c1.distance < c2.distance;

}

int64 evalDetectSift(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints, \
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path, \
					 string &_dstKeypointLst_DirFname, \
					 int _nfeatures,int _octave, double _contrastTh, double _edgeTh, double _sigma);
void genStrDstDir(const string &_probeImgPath,const string &_dstDir, const string &_strFeature, const string &_strParam, \
				  string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path, string &_dstKeypointLst_DirFname);
int64 evalDetectSurf(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints, \
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path, \
					 string &_dstKeypointLst_DirFname, \
					 double _hessianThreshold,int _octave, int _octaveLayers, bool _extended, bool _upright);
/*!
* @brief 特徴点検出→特徴抽出→マッチング、またそれぞれの時間計測
* @brief SIFT/SURF をコマンド引数
* @param [srcImg]
* @param [descriptorPath]
*/
int main( int argc, char *argv[] ){
	stringstream ss;
	//argcは引数の数+1
	if(argc != 5)
	{
		printf("ProgramName: \n");
		printf("usage: %s [probeImgPath] [gallaryImgPath] [detectorNum] [descriptorNum] [dstMatchingImg] [dstProbeFeatures] [dstGallaryFeatures] [dstMatchingResults] [numOfMatchesToDraw]\n", argv[0]);
		printf("      [probeImgPath]        ....... 特徴点検出をする画像のファイルパス(probe)\n");
		printf("      [dstDir]              ....... 特徴点検出をした結果を描画した画像の格納先(ディレクトリ)\n");
		//printf("      [dstFolder]             ....... 出力ファイルのパラメータ（格納先フォルダ名とファイル名にする。magazines_siftなど）\n");
		printf("      [detectorNum]         ....... 検出器の種類を示す番号 \n");
		printf("                            ....... 0:SIFT\n");
		printf("                            ....... 1:SURF\n");
		printf("                            ....... 2:STAR\n");
		printf("                            ....... 3:ORB\n");
		printf("                            ....... 4:FAST\n");
		printf("                            ....... 5:MSER\n");
		printf("                            ....... \n");
		//printf("      [TimeLog]             ....... 処理にかかった時間\n");
		printf("      [dstKeypointLst]      ....... Keypointsリストの出力先(拡張子.ymlは抜き)\n");
		

		exit(EXIT_FAILURE);
	}
	
	//cv::Mat probe_gray_img = cv::imread("C:\\Users\\ZQ83219\\Documents\\Visual Studio 2010\\Projects\\Sandbox\\FeatureExtractionTest\\FeatureMatchingTest\\output\\lena_out.jpg",0);
	string probeImgPath = argv[1];
	cv::Mat probe_gray_img = cv::imread(probeImgPath,0);
	if(probe_gray_img.empty())
	{
		exit(EXIT_FAILURE);
	}
	//imshow("probe",probe_gray_img);
	//waitKey(2);
	string dstDir = argv[2];
	//string dstFolder = argv[3];
	int detector_type = atoi(argv[3]);
	//string dstTimeLogPath = argv[5];
	string dstKeypointLst = argv[4];

	/* 特徴点検出 */
	vector<vector<KeyPoint>> vecvecKeypoint;
	cout << "detect keypoints start" << endl;
	switch(detector_type)
	{
	case 0:
		{
			/* SIFT */
			cout << "SIFT selected" << endl;

			string dir, fname, ext;
			StdSplitpath(probeImgPath, dir, fname, ext);
			ss.str("");
			ss << dstDir << "\\" << fname ;
			checkMkdir(ss.str());
			ss << "\\SIFT" ;
			checkMkdir(ss.str());
			string dstChildDir = ss.str();

			/* パラメータoctaveの保存フォルダ設定 */
			ss << "\\octave" ;
			checkMkdir(ss.str());
			string dstGrandChildDir = ss.str();
			/* キーポイントの描画結果（拡張子なし） */
			ss.str("");
			ss << dstGrandChildDir << "\\" << fname << "_sift_octave" ;
			string dstDrawKeypointsImg_DirFname = ss.str();
			/* 処理時間計測結果 */
			ss.str("");
			ss << dstGrandChildDir << "\\TimeLog_" << fname << "_sift_octave" << ".txt" ;
			string dstTimeLog_Path = ss.str();
			remove(dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			ss.str("");
			ss << dstGrandChildDir << "\\keypoints_" << fname << "_sift_octave";
			string dstKeypointLst_DirFname = ss.str();
			const int oct_table[] = {2,3,4,5};
			for(int o_idx = 0; o_idx < sizeof(oct_table) / sizeof(int) ; o_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectSift(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, \
					dstTimeLog_Path, dstKeypointLst_DirFname, \
					0, oct_table[o_idx], 0.04, 10, 1.6);
			}
			
			/* edgeThreshold */
			ss.str("");
			ss << dstChildDir << "\\edgeThreshold";
			checkMkdir(ss.str());
			dstGrandChildDir = ss.str();
			/* キーポイントの描画結果（拡張子なし） */
			ss.str("");
			ss << dstGrandChildDir << "\\" << fname << "_sift_edgeThreshold" ;
			dstDrawKeypointsImg_DirFname = ss.str();
			/* 処理時間計測結果 */
			ss.str("");
			ss << dstGrandChildDir << "\\TimeLog_" << fname << "_sift_edgeThreshold" << ".txt" ;
			dstTimeLog_Path = ss.str();
			remove(dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			ss.str("");
			ss << dstGrandChildDir << "\\keypoints_" << fname << "_sift_edgeThreshold";
			dstKeypointLst_DirFname = ss.str();
			const double eTh_table[] = {1,1.2,1.4,1.6,1.8,2,3,4,5,6,7,8,9,10,25,50};
			for(int e_idx = 0; e_idx < sizeof(eTh_table) / sizeof(double); e_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectSift(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, \
					dstTimeLog_Path, dstKeypointLst_DirFname, \
					0, 3, 0.04, eTh_table[e_idx], 1.6);
			}

			/* contrastThreshold */
			dstGrandChildDir = dstChildDir + "\\contrastThreshold";
			checkMkdir(dstGrandChildDir);
			/* キーポイントの描画結果（拡張子なし） */
			dstDrawKeypointsImg_DirFname = dstGrandChildDir + "\\" + fname + "_sift_contrastThreshold" ;
			/* 処理時間計測結果 */
			dstTimeLog_Path              = dstGrandChildDir + "\\TimeLog_" + fname + "_sift_contrastThreshold" + ".txt" ;
			remove(dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			dstKeypointLst_DirFname      = dstGrandChildDir + "\\keypoints_" + fname + "_sift_contrastThreshold";
			const double cTh_table[] = {0.02, 0.04, 0.06, 0.08 , 0.10, 0.12, 0.14, 0.16, 0.18, 0.20, 0.22, 0.24};
			for(int c_idx = 0; c_idx < sizeof(cTh_table) / sizeof(double); c_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectSift(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, \
					dstTimeLog_Path, dstKeypointLst_DirFname, \
					0, 3, cTh_table[c_idx], 10, 1.6);
			}

				/* sigma */
			dstGrandChildDir = dstChildDir + "\\sigma";
			checkMkdir(dstGrandChildDir);
			/* キーポイントの描画結果（拡張子なし） */
			dstDrawKeypointsImg_DirFname = dstGrandChildDir + "\\" + fname + "_sift_sigma" ;
			/* 処理時間計測結果 */
			dstTimeLog_Path              = dstGrandChildDir + "\\TimeLog_" + fname + "_sift_sigma" + ".txt" ;
			remove(dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			dstKeypointLst_DirFname      = dstGrandChildDir + "\\keypoints_" + fname + "_sift_sigma";
			const double sgm_table[] = {0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8};
			//const double sgm_table[] = {0.01,0.04,0.06,0.08,0.10};
			for(int s_idx = 0; s_idx < sizeof(sgm_table) / sizeof(double); s_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectSift(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, \
					dstTimeLog_Path, dstKeypointLst_DirFname, \
					0, 3, 0.04, 10,  sgm_table[s_idx]);
			}

			break;
		}
	case 1:
		{
			/* SURF */
			cout << "SURF selected" << endl;
			string dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname;
			genStrDstDir(probeImgPath, dstDir, "SURF", "hessTh", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const double hess_table[] = {200,300,400,500,600};
			for( int h_idx = 0; h_idx < sizeof(hess_table) / sizeof(double) ; h_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectSurf(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, \
					dstTimeLog_Path, dstKeypointLst_DirFname, \
					hess_table[h_idx], 3, 2, true, true);
			}
			
			genStrDstDir(probeImgPath, dstDir, "SURF", "octave", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const double oct_table[] = {2,3,4,5};
			for( int o_idx = 0; o_idx < sizeof(oct_table) / sizeof(double) ; o_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectSurf(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, \
					dstTimeLog_Path, dstKeypointLst_DirFname, \
					400,oct_table[o_idx] , 2, true, true);
			}

			genStrDstDir(probeImgPath, dstDir, "SURF", "octaveLayers", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const double octL_table[] = {2,3,4,5};
			for( int o_idx = 0; o_idx < sizeof(octL_table) / sizeof(double) ; o_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectSurf(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, \
					dstTimeLog_Path, dstKeypointLst_DirFname, \
					400,3 , octL_table[o_idx], true, true);
			}
			
			break;
		}
	case 2:
		{
			/* STAR */
			cout << "STAR selected" << endl;
			StarFeatureDetector detector(100);
			//detector.detect(probe_gray_img,probe_keypoints);
			break;
		}
	case 3:
		{
			/* ORB */
			OrbFeatureDetector detector;
			cout << "ORB selected" << endl;
			//detector.detect(probe_gray_img,probe_keypoints);
			break;
		}
		//else if(detector_type == 4)
	case 4:
		{
			/* FAST */
			cout << "FAST selected" << endl;
			FastFeatureDetector detector;
			//detector.detect(probe_gray_img,probe_keypoints);
			break;
		}
	case 5:
		{
			/* MSER */
			cout << "MSER selected" << endl;
			MserFeatureDetector detector;
			
			//detector.detect(probe_gray_img, probe_keypoints);
			break;
		}
	}

	
	return 0;
}

/*!
 * @brief SIFTの検出関数を実行し、時間を計測する。
 * @param []
 * @retval []
*/
int64 evalDetectSift(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints, \
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path, \
					 string &_dstKeypointLst_DirFname, \
					 int _nfeatures,int _octave, double _contrastTh, double _edgeTh, double _sigma)
{
	/* 時間計測用変数 */
	stringstream ss;
	const double f = (1000 / getTickFrequency());
	int64 st_time;
	int64 en_time;
	int64 diff_time;

	SiftFeatureDetector detector = SiftFeatureDetector(_nfeatures, _octave, _contrastTh, _edgeTh, _sigma);

	st_time = getTickCount();
	detector.detect(_probe_gray_img, _vecKeypoints);
	en_time = getTickCount();
	diff_time = (int)(en_time - st_time) * f;
	_vecvecKeypoint.push_back(_vecKeypoints);
	
	/* 計測時間の記録 */
	ofstream ofs(_dstTimeLog_Path.c_str(), std::ios::out | std::ios::app);
	if(ofs.fail())
	{
		cerr << "file open failed :" << _dstTimeLog_Path << endl;
		exit(EXIT_FAILURE);
	}
	/* --テキスト書き込み */
	ofs <<"nfeat" << _nfeatures << "_oct" << _octave  \
		<< "_contTh" << std::setw(3) << std::setfill('0') << _contrastTh * 100 << "_edgeTh" << setw(3) << setfill('0') << _edgeTh * 10 \
		<< "_sigma" << setw(3) << setfill('0') << _sigma * 10;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss << "_nft" << _nfeatures << "_oct" << _octave  \
		<< "_cTh" << setw(3) << setfill('0') << _contrastTh * 100 << "_eTh" << setw(3) << setfill('0') << _edgeTh * 10 \
		<< "_sgm" << setw(3) << setfill('0') << _sigma * 10 ;
	string strSiftParam = ss.str();
	ss.str("");
	ss << _dstDrawKeypointsImg_DirFname << strSiftParam \
		<< ".png";
	if(imwrite(ss.str(), dstDrawKeypointsImg) == false)
	{
		cerr << "imwrite failure:" <<  _dstDrawKeypointsImg_DirFname << endl;
		exit(EXIT_FAILURE);
	}
	
	/* キーポイント情報の保存 */
	ss.str("");
	ss << _dstKeypointLst_DirFname << strSiftParam << ".yml";
	FileStorage fs(ss.str(),FileStorage::WRITE);
	write(fs, "keypoints", _vecKeypoints);


	cout << "detect keypoints end: " << diff_time << " msec" << endl;
	return diff_time;
}

/*!
 * @brief  _magazines_sift_contrastThresholdのフォーマットで保存先ディレクトリの文字列を生成
 * @param [_probeImgPath] srcDir/imgName.jpgまでの文字列
 * @param [_dstDir] 保存先RootDir
 * @retval []
*/
void genStrDstDir(const string &_probeImgPath,const string &_dstDir, const string &_strFeature, const string &_strParam, \
				  string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path, string &_dstKeypointLst_DirFname)
{
			string dir, imgName, ext;
			stringstream ss;
			StdSplitpath(_probeImgPath, dir, imgName, ext);
			ss << _dstDir << "\\" << imgName ;
			checkMkdir(ss.str());
			ss << "\\" << _strFeature ;
			checkMkdir(ss.str());
			string dstChildDir = ss.str();
	  				/* param */
			string dstGrandChildDir = dstChildDir + "\\" + _strParam;
			checkMkdir(dstGrandChildDir);
			/* キーポイントの描画結果（拡張子なし） */
			//_magazines_sift_contrastThreshold
			_dstDrawKeypointsImg_DirFname = dstGrandChildDir + "\\" + imgName + "_" + _strFeature + "_" + _strParam  ;
			/* 処理時間計測結果 */
			_dstTimeLog_Path              = dstGrandChildDir + "\\TimeLog_" + imgName + "_" + _strFeature + "_" + _strParam + ".txt" ;
			remove(_dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			_dstKeypointLst_DirFname      = dstGrandChildDir + "\\keypoints_" + imgName + "_" + _strFeature + "_" + _strParam ;
}
/*!
 * @brief Surfの検出関数を実行し、時間を計測する。
 * @param []
 * @retval []
*/
int64 evalDetectSurf(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints, \
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path, \
					 string &_dstKeypointLst_DirFname, \
					 double _hessianThreshold,int _octave, int _octaveLayers, bool _extended, bool _upright)
{
	/* 時間計測用変数 */
	stringstream ss;
	const double f = (1000 / getTickFrequency());
	int64 st_time;
	int64 en_time;
	int64 diff_time;

	SurfFeatureDetector detector = SurfFeatureDetector(_hessianThreshold, _octave, _octaveLayers, _extended, _upright);

	st_time = getTickCount();
	detector.detect(_probe_gray_img, _vecKeypoints);
	en_time = getTickCount();
	diff_time = (int)(en_time - st_time) * f;
	_vecvecKeypoint.push_back(_vecKeypoints);
	
	/* 計測時間の記録 */
	ofstream ofs(_dstTimeLog_Path.c_str(), std::ios::out | std::ios::app);
	if(ofs.fail())
	{
		cerr << "file open failed :" << _dstTimeLog_Path << endl;
		exit(EXIT_FAILURE);
	}
	/* --テキスト書き込み */
	ofs <<"hessTh" << setw(3) << setfill('0') << _hessianThreshold << "_oct" << _octave  \
		<< "_octLay" << std::setw(3) << std::setfill('0') << _octaveLayers << "_ext" <<  _extended \
		<< "_upr"	<< _upright;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss <<"hessTh" << setw(3) << setfill('0') << _hessianThreshold << "_oct" << _octave  \
		<< "_octLay" << std::setw(3) << std::setfill('0') << _octaveLayers << "_ext" <<  _extended \
		<< "_upr"	<< _upright;
	string strSurfParam = ss.str();
	ss.str("");
	ss << _dstDrawKeypointsImg_DirFname << strSurfParam \
		<< ".png";
	if(imwrite(ss.str(), dstDrawKeypointsImg) == false)
	{
		cerr << "imwrite failure:" <<  _dstDrawKeypointsImg_DirFname << endl;
		exit(EXIT_FAILURE);
	}
	
	/* キーポイント情報の保存 */
	ss.str("");
	ss << _dstKeypointLst_DirFname << strSurfParam << ".yml";
	FileStorage fs(ss.str(),FileStorage::WRITE);
	write(fs, "keypoints", _vecKeypoints);


	cout << "detect keypoints end: " << diff_time << " msec" << endl;
	return diff_time;
}

/*!
 * @brief STARの検出関数を実行し、時間を計測する。
 * @param []
 * @retval []
*/
int64 evalDetectSurf(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints, \
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path, \
					 string &_dstKeypointLst_DirFname, \
					 double _hessianThreshold,int _octave, int _octaveLayers, bool _extended, bool _upright)
{
	/* 時間計測用変数 */
	stringstream ss;
	const double f = (1000 / getTickFrequency());
	int64 st_time;
	int64 en_time;
	int64 diff_time;

	StarFeatureDetector detector = StarFeatureDetector(_hessianThreshold, _octave, _octaveLayers, _extended, _upright);

	st_time = getTickCount();
	detector.detect(_probe_gray_img, _vecKeypoints);
	en_time = getTickCount();
	diff_time = (int)(en_time - st_time) * f;
	_vecvecKeypoint.push_back(_vecKeypoints);
	
	/* 計測時間の記録 */
	ofstream ofs(_dstTimeLog_Path.c_str(), std::ios::out | std::ios::app);
	if(ofs.fail())
	{
		cerr << "file open failed :" << _dstTimeLog_Path << endl;
		exit(EXIT_FAILURE);
	}
	/* --テキスト書き込み */
	ofs <<"hessTh" << setw(3) << setfill('0') << _hessianThreshold << "_oct" << _octave  \
		<< "_octLay" << std::setw(3) << std::setfill('0') << _octaveLayers << "_ext" <<  _extended \
		<< "_upr"	<< _upright;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss <<"hessTh" << setw(3) << setfill('0') << _hessianThreshold << "_oct" << _octave  \
		<< "_octLay" << std::setw(3) << std::setfill('0') << _octaveLayers << "_ext" <<  _extended \
		<< "_upr"	<< _upright;
	string strSurfParam = ss.str();
	ss.str("");
	ss << _dstDrawKeypointsImg_DirFname << strSurfParam \
		<< ".png";
	if(imwrite(ss.str(), dstDrawKeypointsImg) == false)
	{
		cerr << "imwrite failure:" <<  _dstDrawKeypointsImg_DirFname << endl;
		exit(EXIT_FAILURE);
	}
	
	/* キーポイント情報の保存 */
	ss.str("");
	ss << _dstKeypointLst_DirFname << strSurfParam << ".yml";
	FileStorage fs(ss.str(),FileStorage::WRITE);
	write(fs, "keypoints", _vecKeypoints);


	cout << "detect keypoints end: " << diff_time << " msec" << endl;
	return diff_time;
}