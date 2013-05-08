
#include <iostream>
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <boost/random.hpp>
#include <boost/format.hpp>
#include <opencv2/legacy/legacy.hpp>//OpenCV2.4.5�ȍ~�ł͏��Ȃ��Ƃ��K�v
#include <opencv2/gpu/gpu.hpp>
#include <algorithm>
//#include "KAZE.h"
//#include "config.h"
//#include "utils.h"

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
static bool isDMatchZeroDistance(const DMatch &m)
{
	//return m.distance == 0.0f;
	return m.distance < FLT_EPSILON;

}
static bool isDMatch1BiggerThan2(const DMatch &c1, const DMatch &c2)
{
	return c1.distance < c2.distance;

}
/*!
* @brief �����_���o���������o���}�b�`���O�A�܂����ꂼ��̎��Ԍv��
* @brief SIFT/SURF ���R�}���h����
* @param [srcImg]
* @param [descriptorPath]
*/
int main( int argc, char *argv[] ){
	//argc�͈����̐�+1
	if(argc!=10)
	{
		printf("ProgramName: \n");
		printf("usage: %s [probeImgPath] [gallaryImgPath] [detectorNum] [descriptorNum] [dstMatchingImg] [dstProbeFeatures] [dstGallaryFeatures] [dstMatchingResults] [numOfMatchesToDraw]\n", argv[0]);
		printf("      [probeImgPath]        ....... �������o������摜�̃t�@�C���p�X(probe)\n");
		printf("      [gallaryImgPath]      ....... �������o������摜�̃t�@�C���p�X(gallaryImgPath)\n");
		printf("      [detectorNum]         ....... ���o��̎�ނ������ԍ� \n");
		printf("                            ....... 0:SIFT\n");
		printf("                            ....... 1:SURF\n");
		printf("                            ....... 2:STAR\n");
		printf("                            ....... 3:ORB\n");
		printf("                            ....... 4:FAST\n");
		printf("                            ....... 5:MSER\n");
		printf("                            ....... \n");
		printf("      [descriptorNum]       ....... �������o�A���S���Y���̎�ނ������ԍ� \n");
		printf("                            ....... 0:SIFT\n");
		printf("                            ....... 1:SURF\n");
		printf("                            ....... 2:ORB\n");
		printf("                            ....... 3:Calonder\n");
		printf("                            ....... 4:Brief\n");
		printf("                            ....... 5:HOG\n");
		printf("                            ....... 6:HOG(GPU�g�p�B���o��͑I�ׂȂ��igrid))n");
		printf("      [dstMatchingImg]�@    ....... �}�b�`���O�摜�̏o�͐�(.jpg/.png/...)\n");
		printf("      [dstProbeFeatures]�@  ....... probe������   �̏o�͐�(.xml/.gz)\n");
		printf("      [dstGallaryFeatures]  ....... gallary������ �̏o�͐�(.xml/.gz)\n");
		printf("      [dstMatchingResults]  ....... �}�b�`���O���ʂ̏o�͐�(.xml/.gz)\n");
		printf("      [numOfMatchesToDraw]  ....... �}�b�`���O���ʂ̏o�̓y�A��\n");


		exit(EXIT_FAILURE);
	}
	ofstream ofs("output/output.txt");
	ofs << "test" << endl;
	
	//cv::Mat probe_gray_img = cv::imread("C:\\Users\\ZQ83219\\Documents\\Visual Studio 2010\\Projects\\Sandbox\\FeatureExtractionTest\\FeatureMatchingTest\\output\\lena_out.jpg",0);
	cv::Mat probe_gray_img = cv::imread(argv[1],0);
	cout << argv[1] << endl;
	cv::Mat gallary_gray_img = cv::imread(argv[2],0);
	if(probe_gray_img.empty())
	{
		exit(EXIT_FAILURE);
	}
	if(gallary_gray_img.empty())
	{
		exit(EXIT_FAILURE);
	}
	imshow("probe",probe_gray_img);
	waitKey(2);
	int detector_type = atoi(argv[3]);
	int descriptor_type = atoi(argv[4]);
	string dstMatchingImg = argv[5];
	string dstProbeFeatures = argv[6];
	string dstGallaryFeatures = argv[7];
	string dstMatchingResults = argv[8];
	int numOfMatchesToDraw = atoi(argv[9]);
	/* ���Ԍv���p�ϐ� */
	int64 st_time;
	int64 en_time;
	int64 diff_time;
	const double f = (1000 / getTickFrequency());


	/* �����_���o */
	vector<KeyPoint> probe_keypoints,gallary_keypoints;
	cout << "detect keypoints start" << endl;
	st_time = getTickCount();
	switch(detector_type)
	{
	case 0:
		{
			/* SIFT */
			cout << "SIFT selected" << endl;
			SiftFeatureDetector detector ;
			detector.detect(probe_gray_img,probe_keypoints);
			detector.detect(gallary_gray_img,gallary_keypoints);
			break;
			
		}
	case 1:
		{
			/* SURF */
			cout << "SURF selected" << endl;
			SurfFeatureDetector detector;
			detector.detect(probe_gray_img,probe_keypoints);
			detector.detect(gallary_gray_img,gallary_keypoints);
			break;
		}
	case 2:
		{
			/* STAR */
			cout << "STAR selected" << endl;
			StarFeatureDetector detector(100);
			detector.detect(probe_gray_img,probe_keypoints);
			detector.detect(gallary_gray_img,gallary_keypoints);
			break;
		}
	case 3:
		{
			/* ORB */
			OrbFeatureDetector detector;
			cout << "ORB selected" << endl;
			detector.detect(probe_gray_img,probe_keypoints);
			detector.detect(gallary_gray_img,gallary_keypoints);
			break;
		}
		//else if(detector_type == 4)
	case 4:
		{
			/* FAST */
			cout << "FAST selected" << endl;
			FastFeatureDetector detector;
			detector.detect(probe_gray_img,probe_keypoints);
			detector.detect(gallary_gray_img,gallary_keypoints);
			break;
		}
	case 5:
		{
			/* MSER */
			cout << "MSER selected" << endl;
			MserFeatureDetector detector;
			detector.detect(probe_gray_img, probe_keypoints);
			detector.detect(gallary_gray_img, gallary_keypoints);
			break;
		}
	}
	en_time = getTickCount();
	diff_time = (int)(en_time - st_time) * f;
	cout << "detect keypoints end: " << diff_time << " msec" << endl;


	Mat probe_descriptors,gallary_descriptors;
	gpu::GpuMat gpu_probe_descriptors, gpu_gallary_descriptors;
	cout << "extractor compute start" << endl;
	st_time = getTickCount();
	switch(descriptor_type)
	{
	case _DESCRIPTOR_SIFT:
		{
			/* SIFT */
			cout << "SIFT selected" << endl;

			SiftDescriptorExtractor extractor;
			extractor.compute(probe_gray_img,probe_keypoints,probe_descriptors);
			extractor.compute(gallary_gray_img,gallary_keypoints,gallary_descriptors);
			break;
		}
	case _DESCRIPTOR_SURF:
		{
			/* SURF */ 
			cout << "SURF selected" << endl;
			SurfDescriptorExtractor extractor;
			extractor.compute(probe_gray_img,probe_keypoints,probe_descriptors);
			extractor.compute(gallary_gray_img,gallary_keypoints,gallary_descriptors);
			break;
		}
	case _DESCRIPTOR_ORB:
		{
			/* ORB */
			cout << "ORB selected" << endl;
			OrbDescriptorExtractor extractor;
			//SurfDescriptorExtractor extractor;
			extractor.compute(probe_gray_img,probe_keypoints,probe_descriptors);
			extractor.compute(gallary_gray_img,gallary_keypoints,gallary_descriptors);
			break;
		
		}
	case _DESCRIPTOR_CALONDER:
		{
			/* Calonder */

			cout << "Calonder selected" << endl;
			/* classifierFile�ɂ��Ă̓O�O���Ă����܂�킩��Ȃ������B */
			CalonderDescriptorExtractor<float> extractor = CalonderDescriptorExtractor<float>("output/classifierfile.txt");
			extractor.compute(probe_gray_img,probe_keypoints,probe_descriptors);
			extractor.compute(gallary_gray_img,gallary_keypoints,gallary_descriptors);
			break;
		
		}
	case _DESCRIPTOR_BRIEF:
		{
			/* Brief */
			cout << "Brief selected" << endl;
			BriefDescriptorExtractor extractor(16);
			extractor.compute(probe_gray_img,probe_keypoints,probe_descriptors);
			extractor.compute(gallary_gray_img,gallary_keypoints,gallary_descriptors);
			break;

		}
	case _DESCRIPTOR_HOG: 
		{
			/* HOG */
			cout << "HOG selected" << endl;
			HOGDescriptor extractor;
			// Size(128,64), //winSize
			// Size(16,16), //blocksize
			// Size(8,8), //blockStride,
			// Size(8,8), //cellSize,
			// 9, //nbins,
			// 0, //derivAper,
			// -1, //winSigma,
			// 0, //histogramNormType,
			// 0.2, //L2HysThresh,
			// 0 //gammal correction,
			// //nlevels=64
			//);

			// void HOGDescriptor::compute(const Mat& img, vector<float>& descriptors,
			//                             Size winStride, Size padding,
			//                             const vector<Point>& locations) const
			/* �f�[�^�^�̕ϊ� vector<KeyPoint> -> vector<Point>*/
			vector<float> probe_descriptors_float;
			vector<Point> probe_keypoints_Pt(probe_keypoints.size());
			for(int kp_idx = 0; kp_idx < probe_keypoints.size(); kp_idx++)
			{
				probe_keypoints_Pt[kp_idx].x = (int)(probe_keypoints[kp_idx].pt.x);
				probe_keypoints_Pt[kp_idx].y = (int)(probe_keypoints[kp_idx].pt.y);
			}
			vector<float> gallary_descriptors_float;
			vector<Point> gallary_keypoints_Pt(gallary_keypoints.size());
			for(int kp_idx = 0; kp_idx < gallary_keypoints.size(); kp_idx++)
			{
				gallary_keypoints_Pt[kp_idx].x = (int)(gallary_keypoints[kp_idx].pt.x);
				gallary_keypoints_Pt[kp_idx].y = (int)(gallary_keypoints[kp_idx].pt.y);
			}
			int descriptor_size = extractor.getDescriptorSize();
			extractor.compute(probe_gray_img,probe_descriptors_float,Size(0,0),Size(0,0),probe_keypoints_Pt);
			extractor.compute(gallary_gray_img,gallary_descriptors_float,Size(0,0),Size(0,0),gallary_keypoints_Pt);
		

			cout << endl << "HOG descriptor size is " << descriptor_size << endl;
			cout << "img dimensions: " << probe_gray_img.cols << " width x " << probe_gray_img.rows << "height" << endl;
			cout << "probe:   Found " << probe_descriptors_float.size() << " descriptor values" << endl;
			cout << "probe:   Nr of locations specified : " << probe_keypoints_Pt.size() << endl;
			/* ���Ԍv���p�ϐ� */
			int64 st_time_tmp;
			int64 en_time_tmp;
			int64 diff_time_tmp;
			cout << "�f�[�^�ϊ�(vector<float> -> Mat(CV_32F) start" << endl;
			st_time_tmp = getTickCount();

			/* �����̕ϊ��͂��܂������Ă��Ȃ� */
			/*probe_descriptors = Mat(probe_descriptors_float);
			probe_descriptors = probe_descriptors.reshape(1,probe_keypoints.size());*/
			/* �������ő�p */
			probe_descriptors = Mat(probe_keypoints.size(),descriptor_size,CV_32F);
			for(int i = 0; i < probe_keypoints.size(); i++)
			{
				for(int j = 0 ; j < descriptor_size; j++)
				{
					probe_descriptors.at<float>(i,j) = probe_descriptors_float[i * descriptor_size + j];
				}
			}

			cout << "gallary: Found " << gallary_descriptors_float.size() << " descriptor values" << endl;
			cout << "gallary: Nr of locations specified : " << gallary_keypoints_Pt.size() << endl << endl;

			/* �����̕ϊ��͂��܂������Ă��Ȃ� */
			//gallary_descriptors = Mat(gallary_descriptors_float);
			//gallary_descriptors = gallary_descriptors.reshape(1,gallary_keypoints.size());
			/* �������ő�p */
			gallary_descriptors = Mat(gallary_keypoints.size(),descriptor_size,CV_32F);
			for(int i = 0; i < gallary_keypoints.size(); i++)
			{
				for(int j = 0 ; j < descriptor_size; j++)
				{
					gallary_descriptors.at<float>(i,j) = gallary_descriptors_float[i * descriptor_size + j];
				}
			}

			en_time_tmp = getTickCount();
			diff_time_tmp = (int)(en_time_tmp - st_time_tmp) * f;
			cout << "�f�[�^�ϊ�(vector<float> -> Mat(CV_32F) end: " << diff_time_tmp << " msec" << endl;

			break;

		}
	case _DESCRIPTOR_GPU_HOG:
		{
			cout << "HOG(GPU) selected " << endl;
			gpu::HOGDescriptor hog_gpu;
			gpu::GpuMat gpu_probe_gray_img = probe_gray_img;
			gpu::GpuMat gpu_gallary_gray_img = gallary_gray_img;
			hog_gpu.getDescriptors(gpu_probe_gray_img,Size(0,0),gpu_probe_descriptors);
			hog_gpu.getDescriptors(gpu_gallary_gray_img,Size(0,0),gpu_gallary_descriptors);
			probe_descriptors = gpu_probe_descriptors;
			gallary_descriptors = gpu_gallary_descriptors;
			//hog_gpu.getDescriptors
			break;
		}
	}
	en_time = getTickCount();
	diff_time = (int)(en_time - st_time) * f;
	cout << "extractor compute end: " << diff_time << " msec" << endl;

	/* �}�b�`���O */
	vector<DMatch> matches;
	Mat matches_img;
	// �s��̒��g��float�̏ꍇ��NORM_L2?
	int normType = 
		descriptor_type == _DESCRIPTOR_SIFT     ? NORM_L2 : 
		descriptor_type == _DESCRIPTOR_CALONDER ? NORM_HAMMING : 
		descriptor_type == _DESCRIPTOR_HOG      ? NORM_L2 : 
		NORM_L2;
	BFMatcher matcher = BFMatcher(normType);
	cout << "match start" << endl;
	st_time = getTickCount();
	matcher.match(probe_descriptors,gallary_descriptors,matches);
	
	en_time = getTickCount();
	diff_time = (int)(en_time - st_time) * f;
	cout << "match end: " << diff_time << " msec" << endl;
	
	// �`�悷��y�A�̑I��i���numOfMatchesToDraw��)
	/* distance��0�ȉ��̃y�A���폜 */
	matches.erase(std::remove_if(matches.begin(),matches.end(),isDMatchZeroDistance),matches.end());
	/* distance�Ń\�[�g */
	std::sort(matches.begin(),matches.end(),isDMatch1BiggerThan2);
	/* 臒l�Ń}�X�N */
	float dist_th = matches[numOfMatchesToDraw].distance;
	/*if( matches[0].distance == 0.0f )
	{
		cout << "distance equeals to 0.0f" << endl;
	}*/
	vector<char> matchMask(matches.size());
	for(int i = 0; i < matches.size(); i++)
	{
		matchMask[i] = matches[i].distance < dist_th ? 1 : 0;
	}
	// �Ή��t�����ʂ̕`��
	drawMatches(probe_gray_img,probe_keypoints,gallary_gray_img,gallary_keypoints,matches,matches_img,Scalar::all(-1),Scalar::all(-1),matchMask);

	//�ۑ�����
	FileStorage fs(dstProbeFeatures,FileStorage::WRITE);
	fs << "descriptors" << probe_descriptors;
	fs.release();

	fs.open(dstGallaryFeatures,FileStorage::WRITE);
	fs << "descriptors" << gallary_descriptors ;
	fs.release();

	fs.open(dstMatchingResults,FileStorage::WRITE);
	fs << "matches" << "[";
	for(int i=0;i<matches.size();i++)
	{
		fs << "{:" ;
		fs << "distance" << matches[i].distance;
		fs << "imgIdx"   << matches[i].imgIdx;
		fs << "queryIdx" << matches[i].queryIdx;
		fs << "trainIdx" << matches[i].trainIdx;
		fs << "}";
	}
	fs << "]";
	fs.release();

	// write image file.
	imwrite(dstMatchingImg, matches_img );
	return 0;
}