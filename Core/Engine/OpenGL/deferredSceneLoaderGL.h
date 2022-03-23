#pragma once
#include "Engine/OpenGL/SceneLoaderGL.h"

class deferredSceneLoaderGL : public SceneLoaderGL
{
protected :
	virtual Material* parseMaterial(aiMaterial* mat);
};