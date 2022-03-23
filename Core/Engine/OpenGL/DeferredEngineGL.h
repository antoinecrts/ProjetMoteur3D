/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet, 
 *
 */

#ifndef _DEFERRED_ENGINE_GL_H
#define _DEFERRED_ENGINE_GL_H

#include <map>
#include <string>
#include "Engine/OpenGL/EngineGL.h"

#include "Effects/PostProcess/PostProcess.h"
#include "Engine/OpenGL/SceneLoaderGL.h"

class DeferredEngineGL : public EngineGL
{
	public:
		DeferredEngineGL(int width, int height);
		~DeferredEngineGL();

		virtual bool init(std::string filename = "");
		virtual void render();
		virtual void animate(const int elapsedTime);
		virtual void deferredRender();

		virtual void initGeometryFBO();

protected:

		MaterialGL* def;
		GPUFBO* GeometryPassFBO;
		GPUFBO* PostProcessFBO;
		PostProcess* postProcessing;

	

};
#endif
