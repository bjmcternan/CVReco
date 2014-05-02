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
	CRecoImage();
	~CRecoImage(void);

	void FindFeatures(SIFT& SIFTDetector);
	void FindDescriptors(SIFT& SIFTDetector);
	void Train(string strImagePath, tRecoClassification classification, Mat& response_hist, BOWImgDescriptorExtractor bowide, map<string,Mat>& classes_training_data);
	
	tRecoClassification GetClassification();
	Mat& GetDescriptors(){return this->_Descriptors;};
	void SetFullImagePath(string path);
	string& GetFullImagePath(){return _FullImagePath;};
	void SetClassification(tRecoClassification i);
	bool LoadImg(Mat& rImage);
	vector<KeyPoint>& GetKeyPoints(){return _KeyPoints;};
protected:
	Mat _Descriptors;
	vector<KeyPoint> _KeyPoints;
	tRecoClassification _Classification;
	string _FullImagePath;
};

