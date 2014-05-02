
//LogWriter.cpp : Creates a LogWriter object that prints to a log file
//Code by Brennan McTernan
//4/23/14

#include "RecoLogMgr.h"
#include <time.h>
#include <iostream>
#include <sstream> // for ostringstream

using namespace std;

CRecoLogMgr* CRecoLogMgr::_pInstance = NULL;

CRecoLogMgr* CRecoLogMgr::Instance()
{
	if(!_pInstance)
		_pInstance = new CRecoLogMgr;

	return _pInstance;
}

bool CRecoLogMgr::Init(string strBasePath, string strLogName)
{
	//Constructor - Initializes LogWriter
	//Input - a string containing the filepath
	//Output - none
	//set filepath to be fileLoc
	ostringstream ostrFilePath;
	time_t now = time(NULL);

	ostrFilePath << strBasePath << "\\" << LOGFOLDERNAME << "\\" << strLogName << "_" << now << ".txt";
	try
	{
		_logFile.open(ostrFilePath.str());
	}
	catch ( const exception & ex )
	{
		std::cerr << "Could not open file - reason is " 
                  << ex.what();
		return false;
	}

	return true;
}
CRecoLogMgr::~CRecoLogMgr()
{
	//Destructor - closes the current log file
	//Input: none
	//Output: none
	if(_logFile.is_open())
		_logFile.close();
	//Close log file
}

void CRecoLogMgr::WriteLog(string sizeString)
{
	//WriteLog - Writes a line of text to the log.txt file containing: current timestamp followed by a string 
	//Input: sizeString - a string containing text
	//Output: none
	std::string outString = TimeStamp();
	outString += " " + sizeString;
	_logFile << outString << endl;
	_logFile.flush();
}

std::string CRecoLogMgr::TimeStamp()
{
	//TimeStamp - returns a current localized timestamp string
	//Input: none
	//Output: String containing the current localized time
	struct tm now;
	time_t time;
	_time64(&time);
	char out[100];
	localtime_s(&now,&time);
	asctime_s(out,100,&now);
	out[strnlen_s(out,100)-1] = '\0'; //Trim off newline
	return string(out);
}