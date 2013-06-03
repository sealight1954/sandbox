
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
#define _CERR_DEBUG
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
int64 evalDetectStar(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints, \
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path, \
					 string &_dstKeypointLst_DirFname, \
					 int _maxSize,int _responseThreshold, int _lineThresholdProjected, int _lineThresholdBinarized, int _supressNonMaxSize);
int64 evalDetectOrb(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints,
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path,
					 string &_dstKeypointLst_DirFname,
					 int _nfeatures , 
					 float _scaleFactor , 
					 int _nlevels , 
					 int _edgeThreshold31,
					 int _firstLevel , 
					 int _WTA_K, 
					 int _scoreType,
					 int _patchSize);
int64 evalDetectFast(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints,
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path,
					 string &_dstKeypointLst_DirFname,
					 int _threshold,
					 bool _nonmaxSuppression);
int64 evalDetectMser(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints,
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path,
					 string &_dstKeypointLst_DirFname,
					 int _delta, 
					 int _min_area, 
					 int _max_area,
					 double _max_variation,
					 double _min_diversity,
					 int _max_evolution, 
					 double _area_threshold,
					 double _min_margin, 
					 int _edge_blur_size);
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
			ss << dstGrandChildDir << "\\" << fname << "_SIFT_octave" ;
			string dstDrawKeypointsImg_DirFname = ss.str();
			/* 処理時間計測結果 */
			ss.str("");
			ss << dstGrandChildDir << "\\TimeLog_" << fname << "_SIFT_octave" << ".txt" ;
			string dstTimeLog_Path = ss.str();
			remove(dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			ss.str("");
			ss << dstGrandChildDir << "\\keypoints_" << fname << "_SIFT_octave";
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
			ss << dstGrandChildDir << "\\" << fname << "_SIFT_edgeThreshold" ;
			dstDrawKeypointsImg_DirFname = ss.str();
			/* 処理時間計測結果 */
			ss.str("");
			ss << dstGrandChildDir << "\\TimeLog_" << fname << "_SIFT_edgeThreshold" << ".txt" ;
			dstTimeLog_Path = ss.str();
			remove(dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			ss.str("");
			ss << dstGrandChildDir << "\\keypoints_" << fname << "_SIFT_edgeThreshold";
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
			dstDrawKeypointsImg_DirFname = dstGrandChildDir + "\\" + fname + "_SIFT_contrastThreshold" ;
			/* 処理時間計測結果 */
			dstTimeLog_Path              = dstGrandChildDir + "\\TimeLog_" + fname + "_SIFT_contrastThreshold" + ".txt" ;
			remove(dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			dstKeypointLst_DirFname      = dstGrandChildDir + "\\keypoints_" + fname + "_SIFT_contrastThreshold";
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
			dstDrawKeypointsImg_DirFname = dstGrandChildDir + "\\" + fname + "_SIFT_sigma" ;
			/* 処理時間計測結果 */
			dstTimeLog_Path              = dstGrandChildDir + "\\TimeLog_" + fname + "_SIFT_sigma" + ".txt" ;
			remove(dstTimeLog_Path.c_str());
			/* キーポイント上方の格納先 */
			dstKeypointLst_DirFname      = dstGrandChildDir + "\\keypoints_" + fname + "_SIFT_sigma";
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
			genStrDstDir(probeImgPath, dstDir, "SURF", "hessianThreshold", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
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
			string dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname;
			//StarFeatureDetectordetector = StarFeatureDetector(
			genStrDstDir(probeImgPath, dstDir, "STAR", "maxSize", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int maxSz_table[] = {25, 35, 45, 55, 65};
			for( int sz_idx = 0; sz_idx < sizeof(maxSz_table) / sizeof(int); sz_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectStar(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					maxSz_table[sz_idx], 30, 10, 8, 5);
			}

			genStrDstDir(probeImgPath, dstDir, "STAR", "responseThreshold", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int resTh_table[] = {20, 25, 30, 35, 40, 45, 50};
			for( int rth_idx = 0; rth_idx < sizeof(resTh_table) / sizeof(int); rth_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectStar(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					35, resTh_table[rth_idx], 10, 8, 5);
			}

			genStrDstDir(probeImgPath, dstDir, "STAR", "lineThresholdProjected", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int lThPrj_table[] = {2, 4, 6, 8, 10, 12, 14, 16};
			for( int thprj_idx = 0; thprj_idx < sizeof(lThPrj_table) / sizeof(int); thprj_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectStar(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					35, 30, lThPrj_table[thprj_idx], 8, 5);
			}
			genStrDstDir(probeImgPath, dstDir, "STAR", "lineThresholdBinarized", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int lThBin_table[] = {5, 6, 8, 10, 12, 14, 16};
			for( int thbin_idx = 0; thbin_idx < sizeof(lThBin_table) / sizeof(int); thbin_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectStar(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					35, 30, 10, lThBin_table[thbin_idx], 5);
			}
			genStrDstDir(probeImgPath, dstDir, "STAR", "supressNonMaxSize", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int sup_table[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
			for( int sup_idx = 0; sup_idx < sizeof(sup_table) / sizeof(int); sup_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectStar(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					35, 30, 10, 8, sup_table[sup_idx]);
			}
			//detector.detect(probe_gray_img,probe_keypoints);
			break;
		}
	case 3:
		{
			/* ORB */
			//OrbFeatureDetector detector = ;
			cout << "ORB selected" << endl;
			int nFeatures = 800;
			string dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname;
			//StarFeatureDetectordetector = StarFeatureDetector(
						//		 int _nfeatures = 500, 
					 //float _scaleFactor = 1.2f, 
					 //int _nlevels = 8, 
					 //int _edgeThreshold = 31,
					 //int _firstLevel = 0, 
					 //int _WTA_K=2, 
					 //int _scoreType=ORB::HARRIS_SCORE, 
					 //int _patchSize=31

			genStrDstDir(probeImgPath, dstDir, "ORB", "scaleFactor", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const float scFac_table[] = {0.6f, 0.8f, 1.0f, 1.2f, 1.4f,1.6f};
			for( int sc_idx = 0; sc_idx < sizeof(scFac_table) / sizeof(int); sc_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectOrb(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					nFeatures, scFac_table[sc_idx], 8, 31, 0, 2, ORB::HARRIS_SCORE, 31 );
			}

			genStrDstDir(probeImgPath, dstDir, "ORB", "nlevels", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int nlev_table[] = {2, 4, 6, 8, 10, 12};
			for( int nlv_idx = 0; nlv_idx < sizeof(nlev_table) / sizeof(int); nlv_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectOrb(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					nFeatures, 1.2f, nlev_table[nlv_idx], 31, 0, 2, ORB::HARRIS_SCORE, 31 );
			}

			genStrDstDir(probeImgPath, dstDir, "ORB", "edgeThreshold", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int edge_table[] = {27, 29, 31, 33, 35};
			for( int ed_idx = 0; ed_idx < sizeof(edge_table) / sizeof(int); ed_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectOrb(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					nFeatures, 1.2f,  8, edge_table[ed_idx], 0, 2, ORB::HARRIS_SCORE, 31 );
			}

			genStrDstDir(probeImgPath, dstDir, "ORB", "firstLevel", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int fstLev_table[] = {0,1,2,3};
			for( int fl_idx = 0; fl_idx < sizeof(fstLev_table) / sizeof(int); fl_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectOrb(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					nFeatures, 1.2f, 8, 31, fstLev_table[fl_idx], 2, ORB::HARRIS_SCORE, 31 );
			}
			genStrDstDir(probeImgPath, dstDir, "ORB", "WTAK", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int wta_table[] = {1,2,3,4};
			for( int wta_idx = 0; wta_idx < sizeof(wta_table) / sizeof(int); wta_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectOrb(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					nFeatures, 1.2f, 8, 31, 0, wta_table[wta_idx], ORB::HARRIS_SCORE, 31 );
			}
			genStrDstDir(probeImgPath, dstDir, "ORB", "scoreType", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int score_table[] = {ORB::FAST_SCORE,ORB::HARRIS_SCORE};
			for( int sc_idx = 0; sc_idx < sizeof(score_table) / sizeof(int); sc_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectOrb(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					nFeatures, 1.2f, 8, 31, 0, 2,  score_table[sc_idx], 31 );
			}

			genStrDstDir(probeImgPath, dstDir, "ORB", "patchSize", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int patch_table[] = {27,29,31,33,35};
			for( int pch_idx = 0; pch_idx < sizeof(patch_table) / sizeof(int); pch_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectOrb(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					nFeatures, 1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE, patch_table[pch_idx] );
			}
			//detector.detect(probe_gray_img,probe_keypoints);
			break;
		}
		//else if(detector_type == 4)
	case 4:
		{
			/* FAST */
			cout << "FAST selected" << endl;
			//FastFeatureDetector detector = FastFeatureDetector(;
			string dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname;
			genStrDstDir(probeImgPath, dstDir, "FAST", "threshold", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int th_table[] = {4,6,8,10,12,14,16,18,20,22,24,26,28,30};
			for( int th_idx = 0; th_idx < sizeof(th_table) / sizeof(int); th_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectFast(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					 th_table[th_idx], true );
			}
			genStrDstDir(probeImgPath, dstDir, "FAST", "nonmaxSuppression", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
			const int bool_table[] = {0, 1};
			for( int noMax_idx = 0; noMax_idx < sizeof(bool_table) / sizeof(int); noMax_idx++)
			{
				vector< KeyPoint> probe_keypoints;
				evalDetectFast(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
					 10 , bool_table[noMax_idx] );
			}

			//detector.detect(probe_gray_img,probe_keypoints);
			break;
		}
	case 5:
		{
			/* MSER */
			cout << "MSER selected" << endl;

			//int _delta=5, 
			//	int _min_area=60, 
			//	int _max_area=14400,
			//	double _max_variation=0.25, 
			//	double _min_diversity=.2,
			//	int _max_evolution=200, 
			//	double _area_threshold=1.01,
			//	double _min_margin=0.003, 
			//	int _edge_blur_size=5 )
			string dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname;
			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "delta", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const int val_table[] = {2,3,4,5,6,7,8};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(int); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						val_table[idx] , 60 , 14400 , 0.25 , 0.20 , 200 , 1.01 , 0.003 , 5);
				}
			}
			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "minArea", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const int val_table[] = {20,40,60,80,100,120};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(int); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						5, val_table[idx], 14400 , 0.25 , 0.20 , 200 , 1.01 , 0.003 , 5 );
				}
			}

			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "maxArea", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const int val_table[] = {12000,13200,14400,15600};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(int); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						5, 60 ,val_table[idx],  0.25 , 0.20 , 200 , 1.01 , 0.003 , 5 );
				}
			}
			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "maxVariation", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const double val_table[] = {0.20,0.25,0.30};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(double); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						5, 60 ,14400 , val_table[idx],   0.20 , 200 , 1.01 , 0.003 , 5 );
				}
			}
			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "minDiversity", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const double val_table[] = {0.05,0.10,0.15,0.2,0.25,0.30,0.35,0.40,0.45,0.50};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(double); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						5, 60 ,14400 ,    0.25 ,val_table[idx], 200 , 1.01 , 0.003 , 5 );
				}
			}


			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "maxEvolution", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const int val_table[] = {50,200,350,500};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(int); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						5, 60 , 14400, 0.25 , 0.20 ,val_table[idx] , 1.01 , 0.003 , 5 );
				}
			}
			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "areaThreshold", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const double val_table[] = {0.01,0.10,1.01,5.00,10.00};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(double); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						5, 60 ,14400 , 0.20,   0.25 , 200 ,val_table[idx],  0.003 , 5 );
				}
			}
			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "minMargin", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const double val_table[] = {0.0003,0.001,0.003,0.01,0.03,0.10,0.30};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(double); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						5, 60 ,14400 , 0.20,   0.25 , 200 ,1.01, val_table[idx],   5 );
				}
			}
			{
				genStrDstDir(probeImgPath, dstDir, "MSER", "edgeBlurSize", dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname);
				const int val_table[] = {2,3,4,5,6,7,8,10 ,15,20,25,30,35,40,45,50};
				for( int idx = 0; idx < sizeof(val_table) / sizeof(int); idx++)
				{
					vector< KeyPoint> probe_keypoints;
					evalDetectMser(probe_gray_img, probe_keypoints, vecvecKeypoint, dstDrawKeypointsImg_DirFname, dstTimeLog_Path, dstKeypointLst_DirFname, \
						5, 60 , 14400, 0.25 , 0.20 , 200,1.01 , 0.003 ,val_table[idx]  );
				}
			}
			//MserFeatureDetector detector = MserFeatureDetector(;
			
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
	ofs <<"nfeatures-" << _nfeatures << "_octave-" << _octave  \
		<< "_contrastThreshold-" << setw(3) << setfill('0') << setprecision(2) << fixed << _contrastTh  \
		<< "_edgeThreshold-"     << setw(4) << setfill('0') << setprecision(1) << fixed << _edgeTh \
		<< "_sigma-"             << setw(3) << setfill('0') << setprecision(1) << fixed << _sigma;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss << "_nft" << _nfeatures << "_oct" << _octave  \
		<< "_cTh" << setw(3) << setfill('0') << setprecision(2) << fixed <<_contrastTh \
		<< "_eTh" << setw(4) << setfill('0') << setprecision(1) << fixed << _edgeTh \
		<< "_sgm" << setw(3) << setfill('0') << setprecision(1) << fixed << _sigma ;
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
	ofs <<"hessianThreshold-" << setw(3) << setfill('0') << _hessianThreshold \
		<< "_octave-"                             << _octave  \
		<< "_octaveLayers-"                          << _octaveLayers \
		<< "_extended-"                           <<  _extended \
		<< "_upright-"	                          << _upright;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss <<"_hTh" << setw(3) << setfill('0') << _hessianThreshold \
		<< "_oct" <<                         _octave  \
		<< "_ocL" <<                         _octaveLayers \
		<< "_ext" <<                         _extended \
		<< "_upr" <<                         _upright;
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
int64 evalDetectStar(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints,
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path,
					 string &_dstKeypointLst_DirFname,
					 int _maxSize,int _responseThreshold, 
					 int _lineThresholdProjected, 
					 int _lineThresholdBinarized, 
					 int _supressNonMaxSize)
{
	/* 時間計測用変数 */
	stringstream ss;
	const double f = (1000 / getTickFrequency());
	int64 st_time;
	int64 en_time;
	int64 diff_time;

	StarFeatureDetector detector = StarFeatureDetector(_maxSize, _responseThreshold, _lineThresholdProjected, _lineThresholdBinarized, _supressNonMaxSize);

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
	ofs <<"maxSize-" << setw(2) << setfill('0')                             << _maxSize \
		<< "_responseThreshold-"      << std::setw(2) << std::setfill('0')  << _responseThreshold  \
		<< "_lineThresholdProjected-" << std::setw(2) << std::setfill('0')  << _lineThresholdProjected \
		<< "_lineThresholdBinarized-" << std::setw(2) << std::setfill('0')  <<  _lineThresholdBinarized \
		<< "_supressNonMaxSize-"	                                        << _supressNonMaxSize;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss <<"_maS" << setw(3) << setfill('0') << _maxSize \
		<< "_rTh" << std::setw(2) << std::setfill('0')  << _responseThreshold  \
		<< "_ThP"<< std::setw(2) << std::setfill('0')  << _lineThresholdProjected \
		<< "_thB" << std::setw(2) << std::setfill('0')  <<  _lineThresholdBinarized \
		<< "_spr"	  << _supressNonMaxSize;
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
 * @brief ORBの検出関数を実行し、時間を計測する。
 * @param []
 * @retval []
*/
int64 evalDetectOrb(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints,
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path,
					 string &_dstKeypointLst_DirFname,
					 int _nfeatures = 500, 
					 float _scaleFactor = 1.2f, 
					 int _nlevels = 8, 
					 int _edgeThreshold = 31,
					 int _firstLevel = 0, 
					 int _WTA_K=2, 
					 int _scoreType=ORB::HARRIS_SCORE, 
					 int _patchSize=31
		/*int _maxSize,int _responseThreshold, 
					 int _lineThresholdProjected, 
					 int _lineThresholdBinarized, 
					 int _supressNonMaxSize)*/)
{
	/* 時間計測用変数 */
	stringstream ss;
	const double f = (1000 / getTickFrequency());
	int64 st_time;
	int64 en_time;
	int64 diff_time;
	OrbFeatureDetector detector = OrbFeatureDetector(_nfeatures, _scaleFactor, _nlevels, _edgeThreshold, 
		_firstLevel, _WTA_K, _scoreType, _patchSize);
	//OrbFeatureDetector(
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
	ofs <<"nfeatures-" << setw(3) << setfill('0') << _nfeatures \
		<< "_scaleFactor-" << _scaleFactor \
		<< "_nlevels-" << std::setw(2) << std::setfill('0') << _nlevels \
		<< "_edgeThreshold-" <<  _edgeThreshold \
		<< "_firstLevel-" << _firstLevel \
		<< "_WTAK-" << _WTA_K \
		<< "_scoreType-" << _scoreType \
		<< "_patchSize-" << _patchSize;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();
					 //int _nfeatures = 500, 
					 //float _scaleFactor = 1.2f, 
					 //int _nlevels = 8, 
					 //int _edgeThreshold = 31,
					 //int _firstLevel = 0, 
					 //int _WTA_K=2, 
					 //int _scoreType=ORB::HARRIS_SCORE, 
					 //int _patchSize=31

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss <<"_nft" << setw(3) << setfill('0') << _nfeatures \
		<< "_scl" << setw(2) << setfill('0') << setprecision(1) << fixed << _scaleFactor \
		<< "_nlv" << std::setw(2) << std::setfill('0') << _nlevels \
		<< "_eTh" <<  _edgeThreshold \
		<< "_1Lv" << _firstLevel \
		<< "_WTK" << _WTA_K \
		<< "_scT" << _scoreType \
		<< "_pat" << _patchSize;
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
int64 evalDetectFast(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints,
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path,
					 string &_dstKeypointLst_DirFname,
					 int _threshold,
					 bool _nonmaxSuppression)
{
	/* 時間計測用変数 */
	stringstream ss;
	const double f = (1000 / getTickFrequency());
	int64 st_time;
	int64 en_time;
	int64 diff_time;

	FastFeatureDetector detector = FastFeatureDetector(_threshold , _nonmaxSuppression);

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
	ofs <<"threshold-" << setw(3) << setfill('0') << _threshold \
		<< "_nonmaxSuppression-" << _nonmaxSuppression ;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss <<"_thr" << setw(3) << setfill('0') << _threshold \
		<< "_noS" << _nonmaxSuppression ;
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
 * @brief MSERの検出関数を実行し、時間を計測する。
 * @param []
 * @retval []
*/
int64 evalDetectMser(const Mat& _probe_gray_img, vector<KeyPoint>& _vecKeypoints,
					 vector< vector< KeyPoint > > &_vecvecKeypoint,
					 string &_dstDrawKeypointsImg_DirFname, string &_dstTimeLog_Path,
					 string &_dstKeypointLst_DirFname,
					 int _delta=5, 
					 int _min_area=60, 
					 int _max_area=14400,
					 double _max_variation=0.25, 
					 double _min_diversity=.2,
					 int _max_evolution=200, 
					 double _area_threshold=1.01,
					 double _min_margin=0.003, 
					 int _edge_blur_size=5 )
{
	/* 時間計測用変数 */
	stringstream ss;
	const double f = (1000 / getTickFrequency());
	int64 st_time;
	int64 en_time;
	int64 diff_time;

	MserFeatureDetector detector = MserFeatureDetector(_delta, _min_area, _max_area, 
		_max_variation, _min_diversity, _max_evolution, _area_threshold, _min_margin, _edge_blur_size);

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
	ofs <<"delta-" << setw(1) << setfill('0') << _delta \
		<< "_minArea-" << setw(3) << setfill('0') << _min_area \
		<<"_maxArea-"  << setw(3) << setfill('0') << _max_area \
		<< "_maxVariation-" <<setw(3) << setfill('0') << setprecision(2) << fixed << _max_variation \
		<<"_minDiversity-" << setw(3) << setfill('0') << _min_diversity \
		<< "_maxEvolution-" << setw(3) << setfill('0') << _max_evolution \
		<<"_areaThreshold-" << setw(4) << setfill('0')<< fixed <<setprecision(2) << _area_threshold \
		<< "_minMargin-" << setw(5) << setfill('0')<< fixed <<setprecision(4)<< _min_margin \
		<< "_edgeBlurSize-"<< setw(2) << setfill('0') << _edge_blur_size;
	ofs <<" " << _vecKeypoints.size() << "points " << diff_time << "msec" << endl;
	ofs.close();

	/* 結果の描画・保存 */
	Mat dstDrawKeypointsImg; 
	drawKeypoints(_probe_gray_img, _vecKeypoints, dstDrawKeypointsImg, Scalar(0, 0, 255));
	/* --ファイル名生成 */
	ss.str("");
	/* --ファイルパラメータ生成 */ 
	ss <<"_del" << setw(3)   << setfill('0') << _delta \
		<< "_miA" << setw(3) << setfill('0') << _min_area \
		<<"_maA"  << setw(3) << setfill('0') << _max_area \
		<< "_maV" << setw(3) << setfill('0') << setprecision(2) << fixed << _max_variation \
		<<"_miD" << setw(3)  << setfill('0') << _min_diversity \
		<< "_maE"  << setw(3) << setfill('0') << _max_evolution \
	<<"_aTh" << setw(4)      << setfill('0')<< fixed <<setprecision(2) << _area_threshold \
	<< "_miM"  << setw(5)    << setfill('0')<< fixed <<setprecision(4) << _min_margin \
	<< "_edg"<< setw(2)      << setfill('0') << _edge_blur_size;
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
#ifdef _CERR_DEBUG
	cerr << ss.str() << endl;
#endif
	FileStorage fs(ss.str(),FileStorage::WRITE);
	write(fs, "keypoints", _vecKeypoints);


	cout << "detect keypoints end: " << diff_time << " msec" << endl;
	return diff_time;
}