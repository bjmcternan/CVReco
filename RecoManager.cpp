#include <Windows.h>
#include <iostream>

#include "RecoManager.h"

#define IMAGEFOLDERNAME "Images"
CRecoManager::CRecoManager(void) :
	_bowide(NULL)
{
	Ptr<DescriptorExtractor > extractor(new SiftDescriptorExtractor());
	Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher());
	_bowide = new BOWImgDescriptorExtractor (extractor,matcher);
}


CRecoManager::~CRecoManager(void)
{
	if (_bowide != NULL)
	{
		delete (_bowide) ;
		_bowide = NULL ;
	}

	for (map<string,CvSVM*>::iterator it = _classes_classifiers.begin(); it != _classes_classifiers.end(); ++it) 
	{
		if((*it).second != NULL)
		{
			delete (*it).second;
		}
	}
}

bool CRecoManager::Init(string strBasePath, string strLogName, string numFeatures, string dictionarySize)
{
	if(!CRecoLogMgr::Instance()->Init(strBasePath, strLogName))
		return false;
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Log Created\n");

	ostringstream message;
	message << "RecoManager -> Params used: number of features - " << numFeatures << " Dictionary Size - " << dictionarySize;
	CRecoLogMgr::Instance()->WriteLog(message.str());

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
void CRecoManager::PopulateImages()
{
	//Log
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Populating images : Start ");

	for(unsigned int i = 0; i < NUM_RECO_CLASSIFICATIONS; i++)
	{
		//Train Images
		for(unsigned int j = 0; j < _RecoFileMgr[RECO_TRAIN][i].GetNumImages(); j++)
		{
			CRecoImage tempImage;
			tempImage.SetFullImagePath(_RecoFileMgr[RECO_TRAIN][i].GetImagePath(j));
			tempImage.SetClassification((tRecoClassification)i);
			_Images.push_back(tempImage);
		}

		//Test Images
		for(unsigned int j = 0; j < _RecoFileMgr[RECO_TEST][i].GetNumImages(); j++)
		{
			CRecoImage tempImage;
			tempImage.SetFullImagePath(_RecoFileMgr[RECO_TEST][i].GetImagePath(j));
			tempImage.SetClassification((tRecoClassification)i);
			_TestImages.push_back(tempImage);
		}
	}

	//Log
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Populating images : Stop ");
}

CRecoImage* CRecoManager::GetImage(unsigned int i)
{
	if((int)i >= this->GetNumImages())
		return NULL;
	return &(_Images[i]);
}

bool CRecoManager::BuildModel(unsigned int numFeatures, unsigned int dictionarySize)
{
	//Log
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Building Model : Start ");

	//Convert to arg input
	int maxTimes = 10;
	TermCriteria tc(CV_TERMCRIT_ITER, maxTimes, 0.001);
	int retries = 1;
	int flags = KMEANS_PP_CENTERS;

	SIFT SIFTDetector(numFeatures);

	BOWKMeansTrainer bowTrainer(dictionarySize,tc, retries, flags);
	Mat vocabulary;
	//vector<CRecoImage> _images; 
	
	for(int i = 0; i < this->GetNumImages(); i++)
	{
		float percent = ((float)i / (float)_Images.size()) * 100.0f;
		cout << "Finding Features: " << percent << "%" << "\r";
		_Images[i].FindDescriptors(SIFTDetector);
		bowTrainer.add(_Images[i].GetDescriptors());

		//Log
		ostringstream message;
		message << "RecoManager -> Descriptors for " << _Images[i].GetFullImagePath() << " written. Number: " << i+1;
		CRecoLogMgr::Instance()->WriteLog(message.str());
	}
	cout << "Finding Features: 100%" << endl;
	vocabulary = bowTrainer.cluster();
	
	//Log
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Building Model : Stop ");

	TrainSVM(vocabulary);
	return true;
}

void CRecoManager::TrainSVM(Mat vocabulary)
{
	_bowide->setVocabulary(vocabulary);

	//Get Histograms
	map<string,Mat> classes_training_data; classes_training_data.clear();
	Mat response_hist;
	for(int i = 0; i < _Images.size(); i++)
	{
		float percent = ((float)i / (float)_Images.size()) * 100.0f;
		cout << "Computing Bag Of Words:" << percent << "%" << "\r";
		Mat tempImageMat;
		_Images[i].LoadImg(tempImageMat);
		_bowide->compute(tempImageMat, _Images[i].GetKeyPoints(), response_hist);
		if(classes_training_data.count(RECO_CLASSIFICATION_NAME[_Images[i].GetClassification()]) == 0) //not yet created...
		{ 
			classes_training_data[RECO_CLASSIFICATION_NAME[_Images[i].GetClassification()]].create(0,response_hist.cols,response_hist.type());
		}
		classes_training_data[RECO_CLASSIFICATION_NAME[_Images[i].GetClassification()]].push_back(response_hist);
	}
	cout << "Computing Bag Of Words: 100%" << endl;

	cout << "Training Classifiers" << endl;

	//Log
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Building SVM Classifiers : Start ");

	//Code taken from https://github.com/royshil/FoodcamClassifier/blob/master/main.cpp Lines 117 - 141
	//Slightly modified
	CvSVM mySVM;
	/*Mat a;
	Mat b;
	mySVM.train(a,b);*/

	CvSVMParams params;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	for (map<string,Mat>::iterator it = classes_training_data.begin(); it != classes_training_data.end(); ++it) 
	{
		string class_ = (*it).first;
		cout << "Training Class: " << class_ << ".." << endl;

		Mat samples(0,response_hist.cols,response_hist.type());
		Mat labels(0,1,CV_32FC1);

		//copy class samples and label
		samples.push_back(classes_training_data[class_]);
		Mat class_label = Mat::ones(classes_training_data[class_].rows, 1, CV_32FC1);
		labels.push_back(class_label);

		//copy rest samples and label
		for (map<string,Mat>::iterator it1 = classes_training_data.begin(); it1 != classes_training_data.end(); ++it1) 
		{
			string not_class_ = (*it1).first;
			if(not_class_[0] == class_[0]) continue;
			samples.push_back(classes_training_data[not_class_]);
			class_label = Mat::zeros(classes_training_data[not_class_].rows, 1, CV_32FC1);
			labels.push_back(class_label);
		}

		Mat samples_32f; 
		samples.convertTo(samples_32f, CV_32F);
		if(_classes_classifiers.count(class_) == 0) 
			_classes_classifiers[class_] = new CvSVM(samples_32f, labels);

		CvSVM* p = _classes_classifiers[class_];
		p->train(samples_32f,labels);
		

		/*
		CvSVM svm;
		try
		{
		svm.train(samples_32f,labels);
		}
		catch ( cv::Exception & e )
		{
		 cout << e.what() << endl;
		}*/
	}
	cout << "Training Classifiers: End" << endl;

	//Log
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Building SVM Classifiers : Stop ");
}

bool CRecoManager::TestModel(unsigned int numFeatures)
{
	cout << "Testing Classifiers" << endl;

	//Log
	CRecoLogMgr::Instance()->WriteLog("RecoManager -> Testing Model : Start ");

	SIFT SIFTDetector(numFeatures);
	Mat response_hist;
	int correct = 0;
	for(int i = 0; i < _TestImages.size(); i++)
	{
		Mat tempHist;
		float percent = ((float)i / (float)_Images.size()) * 100.0f;
		cout << "Testing:" << percent << "%" << "\r";
		Mat tempImageMat;
		_TestImages[i].FindFeatures(SIFTDetector);
		_TestImages[i].LoadImg(tempImageMat);
		_bowide->compute(tempImageMat, _TestImages[i].GetKeyPoints(), tempHist);

		
		vector <float> results;
		results.resize ((int)NUM_RECO_CLASSIFICATIONS) ;
		ostringstream message;
		message << "Test image " << _TestImages[i].GetFullImagePath() << " : ";
		for (int j = 0 ; j < (int)NUM_RECO_CLASSIFICATIONS ; j++)
		{
			CvSVM* p = _classes_classifiers[RECO_CLASSIFICATION_NAME[j]];
			results[j] = p->predict(tempHist,true);
			message << RECO_CLASSIFICATION_NAME[j] << " = " << results[j] << " - ";
		}

		float maxres = results[0] ;
		int guess = 0 ;
		for (int j = 1 ; j < (int)NUM_RECO_CLASSIFICATIONS ; j++)
		{
			if (results[j] > maxres)
			{
				maxres = results[j] ;
				guess = j ;
			}
		}

		message << "Guess: " << RECO_CLASSIFICATION_NAME[guess];
		//Log
		CRecoLogMgr::Instance()->WriteLog(message.str());

		if(guess == (int)(_TestImages[i].GetClassification()))
		{
			correct++;
		}
	}

	float percentage = ((float)correct/(float)_TestImages.size()) * 100; 
	cout << "DONE: Percent right : " << percentage << endl;

	ostringstream message;
	message << "~DONE~ Percent right: " << percentage << "%"<< endl;
	CRecoLogMgr::Instance()->WriteLog(message.str());
	return true;
}
