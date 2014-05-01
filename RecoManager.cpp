#include <Windows.h>
#include <iostream>

#include "RecoManager.h"

#define IMAGEFOLDERNAME "Images"
CRecoManager::CRecoManager(void)
{
}


CRecoManager::~CRecoManager(void)
{
}

bool CRecoManager::Init(string strBasePath, string strLogName)
{
	if(!CRecoLogMgr::Instance()->Init(strBasePath, strLogName))
		return false;
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Log Created\n");

	ostringstream strFullPath;

	for(int i = 0; i < NUM_RECO_PROCESS; i++)
	{
		for(int j = 0; j < NUM_RECO_CLASSIFICATIONS; j++)
		{
			// Clear strFullPath
			strFullPath.str("");
			strFullPath.clear();
			//
			//Create new strFullPath
			strFullPath << strBasePath << "\\" << IMAGEFOLDERNAME << "\\" << RECO_PROCESS_NAME[i] << "\\" << RECO_CLASSIFICATION_NAME[j] << "\\";
			
			CRecoLogMgr::Instance()->WriteLog("RecoManager -> New RecoFileMgr. Folder is: " + strFullPath.str());


			if(!_RecoFileMgr[i][j].Init(strFullPath.str()))
			{
				CRecoLogMgr::Instance()->WriteLog("!FAILURE! RecoManager -> New RecoFileMgr failed. Path is: " + strFullPath.str() + "!FAILURE");
				return false;
			}
		}
	}
	return true;
}

bool CRecoManager::BuildModel(unsigned int numFeatures)
{
	//Convert to arg input
	int dictionarySize = 124;
	int maxTimes = 10;
	TermCriteria tc(CV_TERMCRIT_ITER, maxTimes, 0.001);
	int retries = 1;
	int flags = KMEANS_PP_CENTERS;

	BOWKMeansTrainer bowTrainer(dictionarySize,tc, retries, flags);
	Mat vocabulary;
	//vector<CRecoImage> images; 
	
	for(int i = 0; i < NUM_RECO_CLASSIFICATIONS; i++)
	{
		unsigned int count = 0;
		for(unsigned int j = 0; j < _RecoFileMgr[RECO_TRAIN][i].GetNumImages(); j++)
		{
			CRecoImage tempImage;
			tempImage.FindFeatures(_RecoFileMgr[RECO_TRAIN][i].GetImagePath(j), numFeatures);
			//images.push_back(tempImage);
			bowTrainer.add(tempImage.GetDescriptors());
			count++;
		}
		ostringstream message;
		message << "RecoManager -> Descriptors for " << RECO_CLASSIFICATION_NAME[i] << " written. Number: " << count;
		CRecoLogMgr::Instance()->WriteLog(message.str());
		cout << count << endl;
	}
	
	vocabulary = bowTrainer.cluster();

	return true;
}
bool CRecoManager::AssessModel()
{
	return true;
}
bool CRecoManager::EvaluateModel()
{
	return true;
}
