#include "AntiAliasing.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"


AntiAliasing::AntiAliasing(std::string name,int width, int height) :
	EffectGL(name,"PostProcess/AntiAliasing")
{



	fbo_width = width;
	fbo_height = height;

	Base = new GLProgram(this->m_ClassName + "-Base", GL_VERTEX_SHADER);
	m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT, Base);
		
	AA_PRG_Edge_EdgeDetection = new GLProgram(this->m_ClassName + "-AA-Edge-EdgeDetection", GL_FRAGMENT_SHADER);
	AA_PRG_Edge_HBlur = new GLProgram(this->m_ClassName + "-AA-Edge-HBlur", GL_FRAGMENT_SHADER);
	AA_PRG_Edge_VBlur = new GLProgram(this->m_ClassName + "-AA-Edge-VBlur", GL_FRAGMENT_SHADER);
	AA_PRG_Shading_HBlur = new GLProgram(this->m_ClassName + "-AA-Shading-HBlur", GL_FRAGMENT_SHADER);
	AA_PRG_Shading_VBlur = new GLProgram(this->m_ClassName + "-AA-Shading-VBlur", GL_FRAGMENT_SHADER);
	AA_PRG_AntiAliasing = new GLProgram(this->m_ClassName + "-AA-AntiAliasing", GL_FRAGMENT_SHADER);



	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Edge_EdgeDetection);
	m_ProgramPipeline->link();
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Edge_HBlur);
	m_ProgramPipeline->link();
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Edge_VBlur);
	m_ProgramPipeline->link();
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Shading_HBlur);
	m_ProgramPipeline->link();
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Shading_VBlur);
	m_ProgramPipeline->link();
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_AntiAliasing);
	m_ProgramPipeline->link();


	createFBO();
	createSamplers();
	createParameters();

}

void AntiAliasing::createFBO()
{
	AA_FBO_Edge = new GPUFBO("AA_FBO_Edge");
	AA_FBO_Edge_Blur[0] = new GPUFBO("AA_FBO_Edge_HBlur");
	AA_FBO_Edge_Blur[1] = new GPUFBO("AA_FBO_Edge_VBlur");
	AA_FBO_Shading_Blur[0] = new GPUFBO("AA_FBO_Shading_HBlur");
	AA_FBO_Shading_Blur[1] = new GPUFBO("AA_FBO_Shading_VBlur");

	AA_FBO_Edge->createTexture2DAttachments(fbo_width, fbo_height, 1, false, GL_R16F);
	AA_FBO_Edge_Blur[0]->createTexture2DAttachments(fbo_width, fbo_height, 1, false);
	AA_FBO_Edge_Blur[1]->createTexture2DAttachments(fbo_width, fbo_height, 1, false);
	AA_FBO_Shading_Blur[0]->createTexture2DAttachments(fbo_width, fbo_height, 1, false);
	AA_FBO_Shading_Blur[1]->createTexture2DAttachments(fbo_width, fbo_height, 1, false);
}
void AntiAliasing::createSamplers()
{
	AA_Sampler_Edge_EdgeDetection_GBuffers = AA_PRG_Edge_EdgeDetection->uniforms()->getGPUsampler("GBuffers");
	AA_Sampler_Edge_EdgeDetection_Depth = AA_PRG_Edge_EdgeDetection->uniforms()->getGPUsampler("DepthSampler");
	AA_Sampler_Edge_HBlur_Edge = AA_PRG_Edge_HBlur->uniforms()->getGPUsampler("EdgeSampler");
	AA_Sampler_Edge_VBlur_Edge = AA_PRG_Edge_VBlur->uniforms()->getGPUsampler("EdgeSampler");
	AA_Sampler_Shading_HBlur_Edge = AA_PRG_Shading_HBlur->uniforms()->getGPUsampler("EdgeSampler");
	AA_Sampler_Shading_HBlur_Shading = AA_PRG_Shading_HBlur->uniforms()->getGPUsampler("ShadingSampler");
	AA_Sampler_Shading_VBlur_Edge = AA_PRG_Shading_VBlur->uniforms()->getGPUsampler("EdgeSampler");
	AA_Sampler_Shading_VBlur_Shading = AA_PRG_Shading_VBlur->uniforms()->getGPUsampler("ShadingSampler");
	AA_Sampler_AntiAliasing_Color = AA_PRG_AntiAliasing->uniforms()->getGPUsampler("ColorSampler");
	AA_Sampler_AntiAliasing_Edge = AA_PRG_AntiAliasing->uniforms()->getGPUsampler("EdgeSampler");
	AA_Sampler_AntiAliasing_Blur = AA_PRG_AntiAliasing->uniforms()->getGPUsampler("BlurSampler");

	AA_Sampler_Edge_EdgeDetection_GBuffers->Set(0);
	AA_Sampler_Edge_EdgeDetection_Depth->Set(1);
	AA_Sampler_Edge_HBlur_Edge->Set(0);
	AA_Sampler_Edge_VBlur_Edge->Set(0);
	AA_Sampler_Shading_HBlur_Edge->Set(0);
	AA_Sampler_Shading_HBlur_Shading->Set(1);
	AA_Sampler_Shading_VBlur_Edge->Set(0);
	AA_Sampler_Shading_VBlur_Shading->Set(1);
	AA_Sampler_AntiAliasing_Color->Set(0);
	AA_Sampler_AntiAliasing_Edge->Set(1);
	AA_Sampler_AntiAliasing_Blur->Set(2);
}


void AntiAliasing::createParameters()
{
	
	aaEdgeHStep = AA_PRG_Edge_EdgeDetection->uniforms()->getGPUfloat("HStep");
	aaEdgeVStep = AA_PRG_Edge_EdgeDetection->uniforms()->getGPUfloat("VStep");
	aaEdgeHBlurHStep = AA_PRG_Edge_HBlur->uniforms()->getGPUfloat("HStep");
	aaEdgeVBlurVStep = AA_PRG_Edge_VBlur->uniforms()->getGPUfloat("VStep");
	aaShadingHBlurHStep = AA_PRG_Shading_HBlur->uniforms()->getGPUfloat("HStep");
	aaShadingVBlurVStep = AA_PRG_Shading_VBlur->uniforms()->getGPUfloat("VStep");

	aaEdgeHStep->Set(1.0f / AA_FBO_Edge->getWidth());
	aaEdgeVStep->Set(1.0f / AA_FBO_Edge->getHeight());
	aaEdgeHBlurHStep->Set(1.0f / AA_FBO_Edge_Blur[0]->getWidth());
	aaEdgeVBlurVStep->Set(1.0f / AA_FBO_Edge_Blur[1]->getHeight());
	aaShadingHBlurHStep->Set(1.0f / AA_FBO_Shading_Blur[0]->getWidth());
	aaShadingVBlurVStep->Set(1.0f / AA_FBO_Shading_Blur[1]->getHeight());
	
}

AntiAliasing::~AntiAliasing()
{
	delete AA_FBO_Edge;
	delete [] AA_FBO_Edge_Blur;
	delete[] AA_FBO_Shading_Blur;

	delete AA_PRG_Edge_EdgeDetection;		// Edge detection Pass
	delete AA_PRG_Edge_HBlur;				// Edge horizontal blur Pass
	delete AA_PRG_Edge_VBlur;				// Edge vertical blur Pass
	delete AA_PRG_Shading_HBlur;			// Shading blur Pass
	delete AA_PRG_Shading_VBlur;			// Shading blur Pass
	delete AA_PRG_AntiAliasing;				// AntiAliasing Pass (Blur the edges in shaded image)
	delete Base;

}


void AntiAliasing::apply(GPUFBO *GBuffers,GPUFBO* normalizedDepth, GPUFBO* shading, GPUFBO* out)
{
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Edge_EdgeDetection);

	AA_FBO_Edge->enable();
	GBuffers->getColorTexture(0)->bind(AA_Sampler_Edge_EdgeDetection_GBuffers->getValue());
	normalizedDepth->getColorTexture(0)->bind(AA_Sampler_Edge_EdgeDetection_Depth->getValue());

	this->drawQuad();
	normalizedDepth->getColorTexture(0)->release();
	GBuffers->getColorTexture(0)->release();
	AA_FBO_Edge->disable();

	//----- Edge Blur Pass -----//
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Edge_HBlur);

	AA_FBO_Edge_Blur[0]->enable();
	AA_FBO_Edge->getColorTexture(0)->bind(AA_Sampler_Edge_HBlur_Edge->getValue());
	this->drawQuad();
	AA_FBO_Edge->getColorTexture(0)->release();
	AA_FBO_Edge_Blur[0]->disable();

	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Edge_VBlur);

	AA_FBO_Edge_Blur[1]->enable();
	AA_FBO_Edge_Blur[0]->getColorTexture(0)->bind(AA_Sampler_Edge_VBlur_Edge->getValue());
	this->drawQuad();
	AA_FBO_Edge_Blur[0]->getColorTexture(0)->release();
	AA_FBO_Edge_Blur[1]->disable();

	//----- Shading Blur Pass -----//
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Shading_HBlur);

	AA_FBO_Shading_Blur[0]->enable();
	shading->getColorTexture(0)->bind(AA_Sampler_Shading_HBlur_Shading->getValue());
	AA_FBO_Edge_Blur[1]->getColorTexture(0)->bind(AA_Sampler_Shading_HBlur_Edge->getValue());
	this->drawQuad();
	AA_FBO_Edge_Blur[1]->getColorTexture(0)->release();
	shading->getColorTexture(0)->release();
	AA_FBO_Shading_Blur[0]->disable();

	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_Shading_VBlur);

	AA_FBO_Shading_Blur[1]->enable();
	AA_FBO_Shading_Blur[0]->getColorTexture(0)->bind(AA_Sampler_Shading_VBlur_Shading->getValue());
	AA_FBO_Edge_Blur[1]->getColorTexture(0)->bind(AA_Sampler_Shading_VBlur_Edge->getValue());
	this->drawQuad();
	AA_FBO_Edge_Blur[1]->getColorTexture(0)->release();
	AA_FBO_Shading_Blur[0]->getColorTexture(0)->release();
	AA_FBO_Shading_Blur[1]->disable();

	//----- AntiAliasing Pass -----//
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, AA_PRG_AntiAliasing);


	if (out != NULL)
		out->enable();
	shading->getColorTexture(0)->bind(AA_Sampler_AntiAliasing_Color->getValue());
	AA_FBO_Edge_Blur[1]->getColorTexture(0)->bind(AA_Sampler_AntiAliasing_Edge->getValue());
	AA_FBO_Shading_Blur[1]->getColorTexture(0)->bind(AA_Sampler_AntiAliasing_Blur->getValue());

	this->drawQuad();

	AA_FBO_Shading_Blur[1]->getColorTexture(0)->release();
	AA_FBO_Edge_Blur[1]->getColorTexture(0)->release();
	shading->getColorTexture(0)->release();
	if (out != NULL)
		out->disable();


	

}
