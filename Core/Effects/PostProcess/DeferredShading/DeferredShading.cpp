#include "DeferredShading.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"








DeferredShading::DeferredShading(std::string name) :
	EffectGL(name,"PostProcess/DeferredShading")
{

	



	Base = new GLProgram(this->m_ClassName + "-Base", GL_VERTEX_SHADER);
	m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT, Base);

	
		DS_PRG_Shading = new GLProgram(this->m_ClassName + "-DS-Shading", GL_FRAGMENT_SHADER);
	
		DS_PRG_ShadingAO = new GLProgram(this->m_ClassName + "-DS-Shading-AO", GL_FRAGMENT_SHADER);

		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DS_PRG_Shading);
		m_ProgramPipeline->link();

		
	

	createSamplers();
	


}


void DeferredShading::createSamplers()
{
	DS_Sampler_Shading_GBuffers = DS_PRG_Shading->uniforms()->getGPUsampler("GBuffers");
	DS_Sampler_Shading_GBuffers->Set(0);
	DS_Sampler_Shading_GBuffersAO = DS_PRG_Shading->uniforms()->getGPUsampler("GBuffers");
	DS_Sampler_Shading_GBuffersAO->Set(0);
	
	DS_Sampler_Shading_AO = DS_PRG_ShadingAO->uniforms()->getGPUsampler("DOSampler");
	DS_Sampler_Shading_AO->Set(1);
		
}


DeferredShading::~DeferredShading()
{
	
	delete Base;
	delete DS_PRG_Shading;
	delete DS_PRG_ShadingAO;
	
}



void DeferredShading::apply(GPUFBO *GBuffers, GPUFBO* AOBuffer, GPUFBO* out)
{
	if (AOBuffer != NULL)
	{
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DS_PRG_ShadingAO);
		AOBuffer->getColorTexture(0)->bind(DS_Sampler_Shading_AO->getValue());
		GBuffers->getColorTexture(0)->bind(DS_Sampler_Shading_GBuffersAO->getValue());
	}
	else
	{
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DS_PRG_Shading);
		GBuffers->getColorTexture(0)->bind(DS_Sampler_Shading_GBuffers->getValue());
	}
		

	if (out != NULL)
		out->enable();
	
	
		
	this->drawQuad();

	GBuffers->getColorTexture(0)->release();
	if (AOBuffer != NULL)
		AOBuffer->getColorTexture(0)->release();

	
	if (out != NULL)
		out->disable();
	

}

