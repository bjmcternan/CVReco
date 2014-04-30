#include "RecoFileMgr.h"
#include "RecoImage.h"

#include <iostream>
#include <sstream>
#include <direct.h>
#include <Windows.h>

CRecoFileMgr::CRecoFileMgr(void):
	_hFind(INVALID_HANDLE_VALUE)
{
}


CRecoFileMgr::~CRecoFileMgr(void)
{
	if(_hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(_hFind);
		_hFind = INVALID_HANDLE_VALUE;
	}
}

bool CRecoFileMgr::Init(string strBasePath)
{
	WIN32_FIND_DATA ffd;

	ostringstream strPathBuffer;
	ostringstream strDirPath;

	//Build path for FindFirstFile
	//Check for trailing "\"
	if (strBasePath[strBasePath.length()-1] == '\\')
		strDirPath << strBasePath;
	else
		strDirPath << "\\" << strBasePath;
	
	strPathBuffer << strDirPath.str() << "*";
	////

	//Find first file in path
	_hFind = FindFirstFile(strPathBuffer.str().c_str(), &ffd);
	if(_hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			_ImgPaths.push_back(string(strDirPath.str() + ffd.cFileName));		//Add to List
	}

	//Iterate through all files in path
	while(FindNextFile(_hFind, &ffd) != 0)
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			_ImgPaths.push_back(string(strDirPath.str() + ffd.cFileName));		//Add to List
	}
	return true;
}

unsigned int CRecoFileMgr::GetNumImages()
{
	return (int) _ImgPaths.size();
}

string CRecoFileMgr::GetImagePath(unsigned int i)
{
	if(i >= this->GetNumImages())
		return "INVALID";
	return _ImgPaths[i];
}