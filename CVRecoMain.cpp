#include "RecoManager.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

#define NUMARGUMENTS 3
void Usage(char* argv[])
{
	cout << "Usage: " << argv[0] << " <Base Folder Path> <number of features to detect>" << endl;
	exit(-2);
}

int main(int argc, char* argv[])
{
	//Check usage
	if(argc != NUMARGUMENTS)
		Usage(argv);

	CRecoManager recoMgr;
	
	if(!recoMgr.Init(string (argv[1]), "CVRecoMgr")) //Initialize
		return -1;
	if(!recoMgr.BuildModel((unsigned int)atoi(argv[2])))
		return -1;
	if(!recoMgr.AssessModel())
		return -1;
	if(!recoMgr.EvaluateModel())
		return -1;
	
	return 0;
}