#pragma once
//CRecoLogMgr.h : header file for LogWriter.cpp
//Code by Brennan McTernan
//4/23/14

#include <string>
#include <fstream>

using namespace std;


#define LOGFOLDERNAME "Logs"

class CRecoLogMgr
{
public: 
	~CRecoLogMgr();

	static CRecoLogMgr* Instance();
	bool Init(string strBasePath, string strLogName);
	void WriteLog(string);
private:
	CRecoLogMgr(){};  //Private so it can't be called
	CRecoLogMgr(CRecoLogMgr const&){}; //Copy Constructor
	CRecoLogMgr& operator=(CRecoLogMgr const&){}; //Make assignment operator private
	static CRecoLogMgr* _pInstance;

	ofstream _logFile;
	string TimeStamp();
};
