
/*
*	(c) XLim, UMR-CNRS
*	Authors: J.Minier, F.Deves, T.Techene, G.Thoin, G.Gilet
*
*/


#ifndef _GOBLIM_TONEMAPPING__
#define _GOBLIM_TONEMAPPING__


#include "Engine/OpenGL/EffectGL.h"
#include <memory.h>
#include "Engine/OpenGL/ModelGL.h"
#include <time.h>
#include <chrono>



class ToneMapping : public EffectGL
{
	public:
		ToneMapping(std::string name, int width = 1920, int height = 1080, bool _useTemporalTM = false);
		~ToneMapping();

		virtual void apply( GPUFBO* Shading, GPUFBO* out = NULL);

		void createControlBar();
	protected:

		void createSamplers();
		void createBuffers();
		void createParameters();


		GLProgram *TM_PRG_Luminance_Computation;	// Luminance computation Pass (Compute each pixel luminance)
		GLProgram *TM_PRG_Luminance_Downscale;		// Luminance downscale Pass (Compute image max luminance)
		GLProgram *TM_PRG_ToneMapping;				// Tone Mapping (Linearize the scene brightness)	

		GLProgram *Base;

		int fbo_width, fbo_height;

		GPUFBO *TM_FBO_Luminance;
		GPUFBO *TM_FBO_Luminance_Downscale[6];

		GPUsampler *TM_Sampler_Luminance_Computation_HDRLuminance;
		GPUsampler *TM_Sampler_Luminance_Downscale_Luminance;
		GPUsampler *TM_Sampler_ToneMapping_HDR;

		//----- CPU Parameters -----//
		int tmNumDownscaleBuffers;		// Tone Mapping : Number of Buffers used for Luminance Downscale
		float tmWaitTime;				// Tone Mapping : Luminance computation time interval
		float tmLastTime;				// Tone Mapping : Last luminance computation time
		float tmTime;					// Tone Mapping : Current time
		float tmLastMaxLuminance;		// Tone Mapping : Last maximum luminance
		float tmLastPower, tmNewPower;	// Tone Mapping : Powers used in the weighted average new luminance
		float data[4096];				// Tone Mapping : Buffer to store 32x32 luminance texture

		GPUfloat *tmMaxRGBValue;
		GPUfloat *tmExposure;
		GPUfloat *tmLuminanceHStep;
		GPUfloat *tmLuminanceVStep;

		bool useTemporalTM;
		float exposure;
		float lastExposure;
		
		

};
#endif
