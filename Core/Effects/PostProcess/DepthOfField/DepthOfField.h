
/*
*	(c) XLim, UMR-CNRS
*	Authors: J.Minier, F.Deves, T.Techene, G.Thoin, G.Gilet
*
*/


#ifndef _GOBLIM_DEPTH_OF_FIELD__
#define _GOBLIM_DEPTH_OF_FIELD__


#include "Engine/OpenGL/EffectGL.h"
#include <memory.h>
#include "Engine/OpenGL/ModelGL.h"
#include <time.h>
#include <chrono>

class DepthOfField : public EffectGL
{
	public:
		DepthOfField(std::string name,int width = 1920,int height = 1080,float near_Focal = 1.0f,float far_Focal = 64.0f);
		~DepthOfField();

		virtual void apply(GPUFBO *Depth, GPUFBO* Shading, GPUFBO* out = NULL);


	protected:

		void createSamplers();
		void createBuffers();
		void createParameters();
	
		

		GLProgram *Base;

		int fbo_width, fbo_height;
		float nearFocal,farFocal;

		bool useNearDOF, useFarDOF;
		int focalBlurSize;
	int shadingBlurSize ;

		GLProgram *DOF_PRG_Focal_Depth;				// Focal depth Pass (Test if a pixel is in focal range)
		GLProgram *DOF_PRG_Focal_HBlur;				// Focal horizontal blur Pass
		GLProgram *DOF_PRG_Focal_VBlur;				// Focal vertical blur Pass
		GLProgram *DOF_PRG_Shading_HBlur;			// Shading horizontal blur Pass
		GLProgram *DOF_PRG_Shading_VBlur;			// Shading vertical blur Pass
		GLProgram *DOF_PRG_DepthOfField;			// Depth of Field Pass (Mix the blurred and original image according to focal range)

		GPUFBO *DOF_FBO_Focal;
		GPUFBO *DOF_FBO_Focal_Blur[2];
		GPUFBO *DOF_FBO_Shading_Blur[2];

		GPUsampler *DOF_Sampler_Focal_Depth_Depth;				
		GPUsampler *DOF_Sampler_Focal_HBlur_Focal;				
		GPUsampler *DOF_Sampler_Focal_VBlur_Focal;				
		GPUsampler *DOF_Sampler_Shading_HBlur_Focal;			
		GPUsampler *DOF_Sampler_Shading_HBlur_Shading;			
		GPUsampler *DOF_Sampler_Shading_VBlur_Focal;				
		GPUsampler *DOF_Sampler_Shading_VBlur_Shading;				
		GPUsampler *DOF_Sampler_DepthOfField_Color;		
		GPUsampler *DOF_Sampler_DepthOfField_Blur;		
		GPUsampler *DOF_Sampler_DepthOfField_Focal;			

		GPUfloat *dofCameraNear;
		GPUfloat *dofCameraFar;

		GPUfloat *dofFocalNear;
		GPUfloat *dofFocalFar;
				
		GPUint *dofFocalHBlurSize;
		GPUfloat *dofFocalHBlurHStep;
		GPUint *dofFocalVBlurSize;
		GPUfloat *dofFocalVBlurVStep;
		GPUint *dofShadingHBlurSize;
		GPUfloat *dofShadingHBlurMultiplier;
		GPUfloat *dofShadingHBlurHStep;
		GPUint *dofShadingVBlurSize;
		GPUfloat *dofShadingVBlurMultiplier;
		GPUfloat *dofShadingVBlurVStep;
		

};
#endif
