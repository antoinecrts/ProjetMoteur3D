#pragma once
/*
*	(c) XLim, UMR-CNRS
*	Authors: G.Gilet
*
*/

#ifndef _SAMPLE_ENGINE_DEFERRED_H
#define _SAMPLE_ENGINE_DEFERRED_H

#include <map>
#include <string>
#include "Engine/OpenGL/EngineGL.h"
#include "Materials/BoundingBoxMaterial/BoundingBoxMaterial.h"

#include "Materials/ColorMaterial/ColorMaterial.h"
#include "Engine/OpenGL/DeferredEngineGL.h"

class SampleEngine_Deferred : public DeferredEngineGL
{
public:
	SampleEngine_Deferred(int width, int height);
	~SampleEngine_Deferred();

	virtual bool init(std::string filename = "");
	virtual void render();


};
#endif