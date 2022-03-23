
/*
*	(c) XLim, UMR-CNRS
*	Authors: J.Minier, F.Deves, T.Techene, G.Thoin, G.Gilet
*
*/


#ifndef _GOBLIM_DEFERRED_SHADING__
#define _GOBLIM_DEFERRED_SHADING__


#include "Engine/OpenGL/EffectGL.h"
#include <memory.h>
#include "Engine/OpenGL/ModelGL.h"

class DeferredShading : public EffectGL
{
	public:
		DeferredShading(std::string name);
		~DeferredShading();

		virtual void apply(GPUFBO *GBuffers, GPUFBO* AOBuffer = NULL, GPUFBO* out = NULL);


	protected:

		void createSamplers();
	
		

		GLProgram *Base;

		int fbo_width, fbo_height;

		GLProgram *DS_PRG_Shading;					// Deferred Shading Pass (Shade the scene using the G-Buffers)
		GLProgram *DS_PRG_ShadingAO;
		GPUsampler *DS_Sampler_Shading_GBuffers,* DS_Sampler_Shading_GBuffersAO;
		GPUsampler *DS_Sampler_Shading_AO;
		

};
#endif
