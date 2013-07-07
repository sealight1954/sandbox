#include "mmplIoUtils.h"
using namespace cv;
namespace MmplIoUtils{
	Rect getRectMask(const Mat &maskImg){
		//imshow("mask",maskImg);
		//waitKey();
		mmpl::image::AreaSegmentSimple segmentor(2000);
		mmpl::image::LabeledImage lab_img;
		mmpl::image::Image mmplMask=mmpl::image::Image(maskImg);
		segmentor.apply(mmplMask,&lab_img);
		int num=lab_img.getRegionNum();
		if(num!=1){
			cerr << "not 1 region segmented"<<endl;

			Rect rect= Rect(0,0,maskImg.cols,maskImg.rows);	
			return rect;

		}else{
			Rect rect=Rect(lab_img.getRectsOfRegions2()[0]);
			return rect;

		}
	}