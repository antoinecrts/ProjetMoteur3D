#include "LinearizeDepth.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"








LinearizeDepth::LinearizeDepth(std::string name) :
	EffectGL(name,"PostProcess/LinearizeDepth")
{

	



	Base = new GLProgram(this->m_ClassName + "-Base", GL_VERTEX_SHADER);
	Depth_PRG_Normalize = new GLProgram(this->m_ClassName + "-Depth-Normalize", GL_FRAGMENT_SHADER);

	m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT, Base);
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, Depth_PRG_Normalize);
	m_ProgramPipeline->link();
		
	

	createSamplers();
	


}


void LinearizeDepth::createSamplers()
{

	Depth_Sampler_Depth = Depth_PRG_Normalize->uniforms()->getGPUsampler("DepthSampler");
	Depth_Sampler_Depth->Set(0);

	depthCameraNear = Depth_PRG_Normalize->uniforms()->getGPUfloat("zNear");
	depthCameraFar = Depth_PRG_Normalize->uniforms()->getGPUfloat("zFar");

	depthCameraNear->Set(Scene::getInstance()->camera()->getZnear());
	depthCameraFar->Set(Scene::getInstance()->camera()->getZfar());
		
}


LinearizeDepth::~LinearizeDepth()
{
	
	delete Base;
	
}



void LinearizeDepth::apply(GPUFBO *GBuffers,  GPUFBO* out)
{
	if (out != NULL)
		out->enable();
	GBuffers->getDepthTexture()->bind(Depth_Sampler_Depth->getValue());
	this->drawQuad();
	GBuffers->getDepthTexture()->release();
	if (out != NULL)
		out->disable();
	

}

