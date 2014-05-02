#pragma once

typedef enum 
{
	RECO_BIKE,					//0
	RECO_CAR,					//1
	RECO_NONE,					//2
	RECO_PERSON,				//3
	NUM_RECO_CLASSIFICATIONS,	//4
	RECO_UNDEFINED				//5
}tRecoClassification;

static char* RECO_CLASSIFICATION_NAME [] =
{
	"bike",
	"cars",
	"none",
	"person"
};