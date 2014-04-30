#pragma once

typedef enum 
{
	RECO_TRAIN,			//0
	RECO_VALIDATION,	//1
	RECO_TEST,			//2
	NUM_RECO_PROCESS	//3
}tRecoProcess;

static char* RECO_PROCESS_NAME [] =
{
	"Train",
	"Validation",
	"Test"
};