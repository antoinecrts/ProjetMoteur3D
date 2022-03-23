
/*
*	(c) XLim, UMR-CNRS
*	Authors: J.Minier, F.Deves, T.Techene, G.Thoin, G.Gilet
*
*/


#ifndef _GOBLIM_ANTIALIASING_
#define _GOBLIM_ANTIALIASING_


#include "Engine/OpenGL/EffectGL.h"
#include <memory.h>
#include "Engine/OpenGL/ModelGL.h"

class AntiAliasing : public EffectGL
{
	public:
		AntiAliasing(std::string name,int width = 1920,int height = 1080);
		~AntiAliasing();

		virtual void apply(GPUFBO *GBuffers, GPUFBO* normalizedDepth, GPUFBO* shading, GPUFBO* out = NULL);

	
	protected:

		void createSamplers();
		void createFBO();
		void createParameters();



		GPUsampler *AA_Sampler_Edge_EdgeDetection_GBuffers;
		GPUsampler *AA_Sampler_Edge_EdgeDetection_Depth;
		GPUsampler *AA_Sampler_Edge_HBlur_Edge;
		GPUsampler *AA_Sampler_Edge_VBlur_Edge;
		GPUsampler *AA_Sampler_Shading_HBlur_Edge;
		GPUsampler *AA_Sampler_Shading_HBlur_Shading;
		GPUsampler *AA_Sampler_Shading_VBlur_Edge;
		GPUsampler *AA_Sampler_Shading_VBlur_Shading;
		GPUsampler *AA_Sampler_AntiAliasing_Color;
		GPUsampler *AA_Sampler_AntiAliasing_Edge;
		GPUsampler *AA_Sampler_AntiAliasing_Blur;

		GPUFBO *AA_FBO_Edge;
		GPUFBO *AA_FBO_Edge_Blur[2];
		GPUFBO *AA_FBO_Shading_Blur[2];

		GLProgram *AA_PRG_Edge_EdgeDetection;		// Edge detection Pass
		GLProgram *AA_PRG_Edge_HBlur;				// Edge horizontal blur Pass
		GLProgram *AA_PRG_Edge_VBlur;				// Edge vertical blur Pass
		GLProgram *AA_PRG_Shading_HBlur;			// Shading blur Pass
		GLProgram *AA_PRG_Shading_VBlur;			// Shading blur Pass
		GLProgram *AA_PRG_AntiAliasing;				// AntiAliasing Pass (Blur the edges in shaded image)



		GPUfloat* aaEdgeHStep ;
		GPUfloat* aaEdgeVStep;
		GPUfloat* aaEdgeHBlurHStep ;
		GPUfloat* aaEdgeVBlurVStep ;
		GPUfloat* aaShadingHBlurHStep ;
		GPUfloat* aaShadingVBlurVStep ;

		GLProgram *Base;

		int fbo_width, fbo_height;
		
		

};
#endif
