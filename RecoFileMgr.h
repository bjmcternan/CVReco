#pragma once
#include <string>
#include <vector>
#include <Windows.h>



using namespace std;

class CRecoFileMgr
{
public:
	CRecoFileMgr(void);
	~CRecoFileMgr(void);
	bool Init(string strBasePath);
	unsigned int GetNumImages();
	string GetImagePath(unsigned int i);
protected:
	HANDLE _hFind;
	vector <string> _ImgPaths;
};

