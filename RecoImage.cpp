#include "RecoImage.h"
#include "RecoLogMgr.h"

#include <iostream>

CRecoImage::CRecoImage(void):
	_Classification(RECO_UNDEFINED)
{
}


CRecoImage::~CRecoImage(void)
{
}

bool CRecoImage::LoadImg(string strImagePath, Mat& rImage)
{
	rImage = imread(strImagePath, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

    if(! rImage.data )                              // Check for invalid input
    {
		CRecoLogMgr::Instance()->WriteLog("CRecoImage -> !FAILURE! Could not open or find the image. Path is: " + strImagePath); 
        return false;
    }

	return true;
}

void CRecoImage::FindFeatures(string strImagePath, unsigned int numFeatures)
{
	vector <KeyPoint> keyPoints;
	Mat image; 
	if(!this->LoadImg(strImagePath, image))
		CRecoLogMgr::Instance()->WriteLog("CRecoImage -> FindFeatures !FAILURE! Could not open or find the image. Path is: " + strImagePath); 

	SIFT SIFTdetector(numFeatures);
	SIFTdetector.detect(image, keyPoints);
	SIFTdetector.compute(image,keyPoints, this->_Descriptors);
}

void CRecoImage::SetClassification(tRecoClassification classification)
{
	_Classification = classification;
}

tRecoClassification CRecoImage::GetClassification()
{
	return _Classification;
}