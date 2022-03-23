/*
EngineGL overloaded for custom rendering
*/
#include "SampleEngine_Deferred.h"

const std::string ressourceModelPath = "/media/giletp01/Dev/Models/Sponza/crytek-sponza/";
const std::string ressourceTexturePath = "/media/giletp01/Dev/Models/Sponza/crytek-sponza/textures/";


SampleEngine_Deferred::SampleEngine_Deferred(int width, int height) :
	DeferredEngineGL(width, height)
{
	LOG_INFO << "# - " << __FUNCTION__ << std::endl;

}

SampleEngine_Deferred::~SampleEngine_Deferred()
{}


bool SampleEngine_Deferred::init(std::string filename)
{
	DeferredEngineGL::init(ressourceModelPath + "sponza.obj");
	
	LOG_INFO << "initialisation complete" << std::endl;
	return(true);
}


void SampleEngine_Deferred::render()
{
	DeferredEngineGL::render();
}


