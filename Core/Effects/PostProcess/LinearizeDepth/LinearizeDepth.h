
/*
*	(c) XLim, UMR-CNRS
*	Authors: J.Minier, F.Deves, T.Techene, G.Thoin, G.Gilet
*
*/


#ifndef _GOBLIM_LINEARIZE_DEPTH__
#define _GOBLIM_LINEARIZE_DEPTH__


#include "Engine/OpenGL/EffectGL.h"
#include <memory.h>
#include "Engine/OpenGL/ModelGL.h"

class LinearizeDepth : public EffectGL
{
	public:
		LinearizeDepth(std::string name);
		~LinearizeDepth();

		virtual void apply(GPUFBO *GBuffers, GPUFBO* out = NULL);


	protected:

		void createSamplers();

		GLProgram *Base;							// Vertex Shader Pass (Darw a screen aligned quad)
		GLProgram *Depth_PRG_Normalize;				// Normalize Depth Buffer

		GPUsampler *Depth_Sampler_Depth;
		GPUfloat *depthCameraNear;
		GPUfloat *depthCameraFar;

	

};
#endif
