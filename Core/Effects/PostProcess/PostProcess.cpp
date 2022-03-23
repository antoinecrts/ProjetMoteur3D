#include "Effects/PostProcess/PostProcess.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"
#include <time.h>
#include <iostream>




PostProcess::PostProcess(std::string name, int _width,int _height) : EffectGL(name, "PostProcess")
{

	this->createFBOs();


	useDO = false;
	useAA = false;
	useMB = false;
	useTM = false;
	useFog = false;
	useBl = false;
	useDOF = false;
	useShadow = true;

	width = _width,
	height = _height;

	
	

	antiAliasing = new AntiAliasing("antiAliasingEffect",width,height);
	dS3HO = new S3HO("S3HOEffect", width, height);
	deferredShading = new DeferredShading("deferredShadingEffect");
	motionBlur = new MotionBlur("MotionBlur");
	depthOfField = new DepthOfField("DepthOfField", width, height);
	toneMapping = new ToneMapping("ToneMapping", width, height);
	linearizeDepth = new LinearizeDepth("LinearizeDepth");

	



}
PostProcess::~PostProcess()
{	
	delete antiAliasing;
	delete dS3HO;
	delete deferredShading;
	delete motionBlur;
	delete depthOfField;
	delete toneMapping;
	delete linearizeDepth;

}

void PostProcess::createPrograms()
{
	/*
	if(useFog)
	{
		FOG_PRG_Fog	= new GLProgram(this->m_ClassName + "-FOG-Fog", GL_FRAGMENT_SHADER);

		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, FOG_PRG_Fog);
		m_ProgramPipeline->link();
	}	
	*/
	
}
void PostProcess::createFBOs()
{
	unsigned int FBOWidth, FBOHeight, i;

	Depth_FBO_Normalized = new GPUFBO("Normalized Depth Buffer");
	Depth_FBO_Normalized->createTexture2DAttachments(FBO_WIDTH, FBO_HEIGHT, 1, false, GL_R32F);

	DS_FBO_Shading	= new GPUFBO("DS_FBO_Shading");
	DS_FBO_Shading->createTexture2DAttachments(FBO_WIDTH, FBO_HEIGHT, 1, false, GL_RGB16F);

	
	S3HO_FBO = new GPUFBO("S3HO_FBO");
	S3HO_FBO->createTexture2DAttachments(FBO_WIDTH, FBO_HEIGHT, 1, false, GL_RGB16F);


}
/*
void PostProcess::createSamplers()
{
	


	
	if(useFog)
	{
		FOG_Sampler_Fog_Color	= FOG_PRG_Fog->uniforms()->getGPUsampler("ColorSampler");
		FOG_Sampler_Fog_Depth	= FOG_PRG_Fog->uniforms()->getGPUsampler("DepthSampler");

		FOG_Sampler_Fog_Color->Set(0);
		FOG_Sampler_Fog_Depth->Set(1);
	}


}


void PostProcess::createGPUParameters(float threshold, int blurSize, float nearFocal, float farFocal, int focalBlurSize, int shadingBlurSize, float exposure)
{

	if(useFog)
	{
		fogZNear= FOG_PRG_Fog->uniforms()->getGPUfloat("zNear");
		fogZFar	= FOG_PRG_Fog->uniforms()->getGPUfloat("zFar");

		fogZNear->Set(Scene::getInstance()->camera()->getZnear());
		fogZFar->Set(Scene::getInstance()->camera()->getZfar());
	}	

}
*/

void PostProcess::apply(GPUFBO *GBuffers, GPUFBO *out)
{	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (m_ProgramPipeline)
	{
		this->applyHDRPostProcess(GBuffers, out);			
	}
	glPopAttrib();
}

void PostProcess::applyHDRPostProcess(GPUFBO *GBuffers, GPUFBO *out)
{

	linearizeDepth->apply(GBuffers, Depth_FBO_Normalized);

	GPUFBO *temp_FBO = NULL;
	if (useMB)
		temp_FBO = DS_FBO_Shading;
	else
		temp_FBO = out;


	if ( useDO )
		dS3HO->apply(GBuffers, Depth_FBO_Normalized, S3HO_FBO);
	
	temp_FBO->enable();
	if (useDO)
		deferredShading->apply(GBuffers, S3HO_FBO, NULL);
	else
		deferredShading->apply(GBuffers, NULL, NULL);


	if ( useAA )
		antiAliasing->apply(GBuffers, Depth_FBO_Normalized, temp_FBO, NULL);
	
	if (useDOF)
		depthOfField->apply(Depth_FBO_Normalized, temp_FBO, NULL);

	
	if (useTM)
		toneMapping->apply(temp_FBO, NULL);
	temp_FBO->disable();

	if (useMB) 
		motionBlur->apply(GBuffers, temp_FBO, out);

	
}

void PostProcess::displayInterface()
{
	/*
	controlBar = TwNewBar("Post Process");
	TwAddVarRW(controlBar, "S3HDO", TW_TYPE_BOOLCPP, &useDO, " label='Direct Occlusion' ");
	TwAddVarRW(controlBar, "TTM", TW_TYPE_BOOLCPP, &useTM, " label='Temporal Tone Mapping' ");
	TwAddVarRW(controlBar, "AA", TW_TYPE_BOOLCPP, &useAA, " label='AntiAliasing' ");
	TwAddVarRW(controlBar, "DOF", TW_TYPE_BOOLCPP, &useDOF, " label='Depth Of Field' ");
	TwAddVarRW(controlBar, "Bloom", TW_TYPE_BOOLCPP, &useBl, " label='Bloom' ");
	TwAddVarRW(controlBar, "MB", TW_TYPE_BOOLCPP, &useMB, " label='MotionBlur' ");
	TwAddVarRW(controlBar, "Fog", TW_TYPE_BOOLCPP, &useFog, " label='Fog' ");
	TwAddVarRW(controlBar, "Ombre", TW_TYPE_BOOLCPP, &useShadow, " label='Shadows' ");
	TwAddButton(controlBar, "Config_S3HDO", openControlBar, dS3HO, " label='S3HDO' group = 'Config' ");
	*/

}



/*
void PostProcess::applyFog(GPUFBO *AA_FBO_Edge, GPUFBO *Shading, GPUFBO *out)
{
	//----- Fog Pass -----//
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, FOG_PRG_Fog);

	out->enable();
		Shading->getColorTexture(0)->bind(FOG_Sampler_Fog_Color->getValue());
		AA_FBO_Edge->getColorTexture(0)->bind(FOG_Sampler_Fog_Depth->getValue());
			this->draw();
		AA_FBO_Edge->getColorTexture(0)->release();
		Shading->getColorTexture(0)->release();
	out->disable();
}
*/

bool PostProcess::isUseShadow() {
	return useShadow;
}

