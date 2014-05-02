#include "RecoManager.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

#define NUMARGUMENTS 3
void Usage(char* argv[])
{
	cout << "Usage: " << argv[0] << " <Base Folder Path> <Name of Log File> <number of features to detect>" << endl;
	exit(-2);
}

int main(int argc, char* argv[])
{
	//Check usage
	if(argc != NUMARGUMENTS+1)
		Usage(argv);

	CRecoManager recoMgr;
	unsigned int numFeatures = (unsigned int)atoi(argv[3]);
	
	if(!recoMgr.Init(string (argv[1]), string(argv[2]), string(argv[3]))) //Initialize
		return -1;

	//Populate images
	cout << "Populating Image List..." << endl;
	recoMgr.PopulateImages();

	cout << "Building Model..." << endl;
	if(!recoMgr.BuildModel(numFeatures))
		return -1;
	if(!recoMgr.TestModel(numFeatures))
		return -1;

	return 0;
}