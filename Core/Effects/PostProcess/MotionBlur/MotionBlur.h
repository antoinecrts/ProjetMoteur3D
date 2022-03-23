
/*
*	(c) XLim, UMR-CNRS
*	Authors: J.Minier, F.Deves, T.Techene, G.Thoin, G.Gilet
*
*/


#ifndef _GOBLIM_MOTION_BLUR__
#define _GOBLIM_MOTION_BLUR__


#include "Engine/OpenGL/EffectGL.h"
#include <memory.h>
#include "Engine/OpenGL/ModelGL.h"
#include <time.h>
#include <chrono>

class MotionBlur : public EffectGL
{
	public:
		MotionBlur(std::string name);
		~MotionBlur();

		virtual void apply(GPUFBO *GBuffers, GPUFBO* Shading, GPUFBO* out = NULL);


	protected:

		void createSamplers();
	
		

		GLProgram *Base;

		int fbo_width, fbo_height;

		GLProgram *MB_PRG_MotionBlur;
		GPUsampler *MB_Sampler_MotionBlur_Color;
		GPUsampler *MB_Sampler_MotionBlur_Velocity;

		float mbTime;
		float mbLastTime;
		int mbNbFrames = 0;
		float lastDelta = 0.0f;
		GPUfloat* velocityScale;
		std::chrono::high_resolution_clock::time_point lastUpdateTime;
		

};
#endif
