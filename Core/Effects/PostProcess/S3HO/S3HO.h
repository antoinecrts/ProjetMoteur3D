
/*
*	(c) XLim, UMR-CNRS
*	Authors: J.Minier, F.Deves, T.Techene, G.Thoin, G.Gilet
*
*/


#ifndef _GOBLIM_S3HO_
#define _GOBLIM_S3HO_


#include "Engine/OpenGL/EffectGL.h"
#include <memory.h>
#include "Engine/OpenGL/ModelGL.h"


class S3HO : public EffectGL
{
	public:
		S3HO(std::string name, int width = 1920, int height = 1080);
		~S3HO();

		virtual void apply(GPUFBO *GBuffers, GPUFBO* normalizedDepth, GPUFBO* out = NULL);

		float doBaseRadiusVal;
		float doMaxOcclusionDistVal;

		bool active;

		void createControlBar();
	
	protected:

		void createSamplers();
		void createFBO();
		void createParameters();
		void setUpParameters();
		void computeNoiseTexture();
		void computeKernels();



		float noiseTexture_numSamples;
		int noiseSize;


		GLProgram *DO_PRG_DO;						// Directional Occlusion Pass
		GLProgram *DO_PRG_DO_HBlur;					// DO horizontal blur Pass
		GLProgram *DO_PRG_DO_VBlur;					// DO vertical blur Pass


		GPUFBO *DO_FBO_DO;
		GPUFBO *DO_FBO_DO_Blur[2];

		GPUsampler *DO_Sampler_DO_GBuffers;
		GPUsampler *DO_Sampler_DO_Depth;
		GPUsampler *DO_Sampler_DO_Noise;
		GPUsampler *DO_Sampler_DO_HBlur_DO;
		GPUsampler *DO_Sampler_DO_HBLUR_GBuffers;
		GPUsampler *DO_Sampler_DO_VBlur_DO;
		GPUsampler *DO_Sampler_DO_VBLUR_GBuffers;


		GPUvec3 *doCamPos;
		GPUint *doHBlurSize;
		GPUfloat *doHBlurHStep;
		GPUint *doVBlurSize;
		GPUfloat *doVBlurVStep;


		GPUfloat *doHStep;
		GPUfloat *doVStep;
		GPUfloat *doBaseRadius;
		GPUfloat *doMaxOcclusionDist;
		GPUint *numSamples;

		GPUTexture2D *noiseTex;


		GPUBuffer *kernels;
		

		GLProgram *Base;

		int fbo_width, fbo_height;

		
		
		

};
#endif
