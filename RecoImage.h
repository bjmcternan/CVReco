#pragma once

#include "RecoClassification.h"
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace std;
using namespace cv;

class CRecoImage
{
public:
	CRecoImage(void);
	~CRecoImage(void);
	
	void FindFeatures(string strImagePath, unsigned int numFeatures);
	void SetClassification(tRecoClassification classification);
	tRecoClassification GetClassification();
	Mat& GetDescriptors(){return _Descriptors;};
protected:
	Mat _Descriptors;
	tRecoClassification _Classification;
	
	bool LoadImg(string strImagePath, Mat& rImage);
};

