#pragma once
#include <string>
#include "RecoLogMgr.h"
#include "RecoFileMgr.h"
#include "RecoImage.h"
#include "RecoClassification.h"
#include "RecoProcess.h"
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

class CRecoManager
{
public:
	CRecoManager(void);
	~CRecoManager(void);
	bool Init(string strBasePath, string strLogName, string numFeatures);
	bool BuildModel(unsigned int);
	bool TestModel(unsigned int numFeatures);
	void PopulateImages();
	int GetNumImages(){return (int)(_Images.size());};
	CRecoImage* GetImage(unsigned int i);
protected:
	CRecoFileMgr _RecoFileMgr[NUM_RECO_PROCESS][NUM_RECO_CLASSIFICATIONS];
	vector<CRecoImage> _Images;
	vector<CRecoImage> _TestImages;
	void TrainSVM(Mat vocabulary);
	BOWImgDescriptorExtractor* _bowide;
	map<string,CvSVM*> _classes_classifiers;
};

