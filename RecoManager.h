#pragma once
#include <string>
#include "RecoLogMgr.h"
#include "RecoFileMgr.h"
#include "RecoImage.h"
#include "RecoClassification.h"
#include "RecoProcess.h"

using namespace std;

class CRecoManager
{
public:
	CRecoManager(void);
	~CRecoManager(void);
	bool Init(string strBasePath, string strLogName);
	bool BuildModel(unsigned int);
	bool AssessModel();
	bool EvaluateModel();

protected:
	CRecoFileMgr _RecoFileMgr[NUM_RECO_PROCESS][NUM_RECO_CLASSIFICATIONS];
	
};

