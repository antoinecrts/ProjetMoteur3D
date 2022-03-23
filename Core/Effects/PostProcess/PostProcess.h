/*
*	(c) XLim, UMR-CNRS
*	Authors: J.Minier, F.Deves, T.Techene, G.Thoin, G.Gilet
*
*/

#ifndef _POSTPROCESS_EFFECT_
#define _POSTPROCESS_EFFECT_

#include "Engine/OpenGL/EffectGL.h"
#include <memory.h>
#include "Engine/OpenGL/ModelGL.h"
#include <omp.h>
#include <time.h>
#include <chrono>
#include "Effects/PostProcess/AntiAliasing/AntiAliasing.h"
#include "Effects/PostProcess/S3HO/S3HO.h"
#include "Effects/PostProcess/DeferredShading/DeferredShading.h"

#include "Effects/PostProcess/MotionBlur/MotionBlur.h"
#include "Effects/PostProcess/DepthOfField/DepthOfField.h"
#include "Effects/PostProcess/ToneMapping/ToneMapping.h"
#include "Effects/PostProcess/LinearizeDepth/LinearizeDepth.h"

//#include "Effects/Shadow/Shadow.h"

class PostProcess : public EffectGL
{
	public:
		PostProcess(std::string name,int _width = 1920,int _height = 1080);
	
		~PostProcess();

		virtual void apply(GPUFBO *in, GPUFBO *out);

		bool isUseShadow();

	protected:

		//----- Effect Initialization Functions -----//
		void createPrograms();
		void createFBOs();
		virtual void displayInterface();
		
		//----- Effect Applying Functions -----//
		//void applyLDRPostProcess(GPUFBO *GBuffers, GPUFBO *out);
		void applyHDRPostProcess(GPUFBO *GBuffers, GPUFBO *out);
		
		//----- Effect Parameters -----//
		bool useHDRPostProcess;	// Use HDR Post Processing (Tone Mapping done last !)
		bool useAA;				// Use AntiAliasing
		bool useTM;				// Use Tone Mapping
		bool useBl;				// Use Bloom
		bool useDOF;			// Use Depth of Field
		bool useFog;			// Use Fog
		bool useMB;				// Use Motion Blur
		bool useDO;				// Use Directional occlusion
		bool useShadow;			// Use Shadow

		
		
		//----- FBOs -----//
		GPUFBO *Depth_FBO_Normalized;
		GPUFBO *S3HO_FBO;		
		GPUFBO *DS_FBO_Shading;

		DeferredShading* deferredShading;
		AntiAliasing* antiAliasing;
		S3HO* dS3HO;
		MotionBlur* motionBlur;
		DepthOfField* depthOfField;
		ToneMapping* toneMapping;
		LinearizeDepth* linearizeDepth;

		// Width and height of PostProcess FBO
		int width, height;



};
#endif