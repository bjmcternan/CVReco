#include "RecoImage.h"
#include "RecoLogMgr.h"

#include <iostream>

CRecoImage::CRecoImage():
	_Classification(RECO_UNDEFINED)
{
}


CRecoImage::~CRecoImage(void)
{
}

void CRecoImage::SetFullImagePath(string path)
{
	_FullImagePath = path;
}

bool CRecoImage::LoadImg(Mat& rImage)
{
	rImage = imread(_FullImagePath, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

    if(! rImage.data )                              // Check for invalid input
    {
		CRecoLogMgr::Instance()->WriteLog("CRecoImage -> !FAILURE! Could not open or find the image. Path is: \"" + _FullImagePath+ "\""); 
        return false;
    }

	return true;
}

void CRecoImage::FindFeatures(SIFT& SIFTDetector)
{
	Mat image; 
	if(!this->LoadImg(image))
		CRecoLogMgr::Instance()->WriteLog("CRecoImage -> FindFeatures !FAILURE! Could not open or find the image. Path is: " + this->GetFullImagePath()); 

	
	SIFTDetector.detect(image, this->_KeyPoints);
}

void CRecoImage::FindDescriptors(SIFT& SIFTDetector)
{
	Mat image; 
	if(!this->LoadImg(image))
		CRecoLogMgr::Instance()->WriteLog("CRecoImage -> FindFeatures !FAILURE! Could not open or find the image. Path is: " + this->GetFullImagePath()); 

	
	SIFTDetector.detect(image, this->_KeyPoints);
	SIFTDetector.compute(image, this->_KeyPoints, this->_Descriptors);
}
//
//void CRecoImage::Train(string strImagePath, tRecoClassification classification, Mat& response_hist, BOWImgDescriptorExtractor bowide, map<string,Mat>& classes_training_data)
//{
//		Mat image; 
//		if(!this->LoadImg(strImagePath, image))
//		CRecoLogMgr::Instance()->WriteLog("CRecoImage -> FindFeatures !FAILURE! Could not open or find the image. Path is: " + strImagePath); 
//
//		bowide.compute(image, keypoints, response_hist);
//
//		if(classes_training_data.count(RECO_CLASSIFICATION_NAME[classification]) == 0) { //not yet created...
//			classes_training_data[RECO_CLASSIFICATION_NAME[classification]].create(0,response_hist.cols,response_hist.type());
//		}
//		classes_training_data[RECO_CLASSIFICATION_NAME[classification]].push_back(response_hist);
//}

void CRecoImage::SetClassification(tRecoClassification i)
{
	_Classification = i;
}

tRecoClassification CRecoImage::GetClassification()
{
	return _Classification;
}