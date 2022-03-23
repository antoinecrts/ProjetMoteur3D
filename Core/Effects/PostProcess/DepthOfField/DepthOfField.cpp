#include "DepthOfField.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"







DepthOfField::DepthOfField(std::string name,int width,int height, float near_Focal ,float far_Focal ):
	EffectGL(name,"PostProcess/DepthOfField")
{

nearFocal = near_Focal;
farFocal = far_Focal;
	useNearDOF = nearFocal > -1.0;
	useFarDOF = farFocal > -1.0;
	fbo_width = width;
	fbo_height = height;
	focalBlurSize = 8.0;
	shadingBlurSize = 8.0;


	Base = new GLProgram(this->m_ClassName + "-Base", GL_VERTEX_SHADER);
	m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT, Base);
	
	if(useNearDOF && useFarDOF)
			DOF_PRG_Focal_Depth	= new GLProgram(this->m_ClassName + "-DOF-Focal-NearFarDepth", GL_FRAGMENT_SHADER);
		else
		{
			if(useNearDOF)
				DOF_PRG_Focal_Depth	= new GLProgram(this->m_ClassName + "-DOF-Focal-NearDepth", GL_FRAGMENT_SHADER);
			if(useFarDOF)
				DOF_PRG_Focal_Depth	= new GLProgram(this->m_ClassName + "-DOF-Focal-FarDepth", GL_FRAGMENT_SHADER);
		}

		DOF_PRG_Focal_HBlur		= new GLProgram(this->m_ClassName + "-DOF-Focal-HBlur", GL_FRAGMENT_SHADER);
		DOF_PRG_Focal_VBlur		= new GLProgram(this->m_ClassName + "-DOF-Focal-VBlur", GL_FRAGMENT_SHADER);
		DOF_PRG_Shading_HBlur	= new GLProgram(this->m_ClassName + "-DOF-Shading-HBlur", GL_FRAGMENT_SHADER);
		DOF_PRG_Shading_VBlur	= new GLProgram(this->m_ClassName + "-DOF-Shading-VBlur", GL_FRAGMENT_SHADER);
		
		DOF_PRG_DepthOfField	= new GLProgram(this->m_ClassName + "-DOF-DepthOfField", GL_FRAGMENT_SHADER);

		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Focal_Depth);
		m_ProgramPipeline->link();
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Focal_HBlur);
		m_ProgramPipeline->link();
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Focal_VBlur);
		m_ProgramPipeline->link();
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Shading_HBlur);
		m_ProgramPipeline->link();
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Shading_VBlur);
		m_ProgramPipeline->link();
		
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_DepthOfField);
		m_ProgramPipeline->link();


	createSamplers();
	createBuffers();
createParameters();

}


void DepthOfField::createSamplers()
{
	
		DOF_Sampler_Focal_Depth_Depth		= DOF_PRG_Focal_Depth->uniforms()->getGPUsampler("DepthSampler");
		DOF_Sampler_Focal_HBlur_Focal		= DOF_PRG_Focal_HBlur->uniforms()->getGPUsampler("FocalSampler");
		DOF_Sampler_Focal_VBlur_Focal		= DOF_PRG_Focal_VBlur->uniforms()->getGPUsampler("FocalSampler");
		DOF_Sampler_Shading_HBlur_Focal		= DOF_PRG_Shading_HBlur->uniforms()->getGPUsampler("FocalSampler");
		DOF_Sampler_Shading_HBlur_Shading	= DOF_PRG_Shading_HBlur->uniforms()->getGPUsampler("ShadingSampler");
		DOF_Sampler_Shading_VBlur_Focal		= DOF_PRG_Shading_VBlur->uniforms()->getGPUsampler("FocalSampler");
		DOF_Sampler_Shading_VBlur_Shading	= DOF_PRG_Shading_VBlur->uniforms()->getGPUsampler("ShadingSampler");
		
		DOF_Sampler_DepthOfField_Color		= DOF_PRG_DepthOfField->uniforms()->getGPUsampler("ColorSampler");
		DOF_Sampler_DepthOfField_Blur		= DOF_PRG_DepthOfField->uniforms()->getGPUsampler("BlurSampler");
		DOF_Sampler_DepthOfField_Focal		= DOF_PRG_DepthOfField->uniforms()->getGPUsampler("FocalSampler");

		DOF_Sampler_Focal_Depth_Depth->Set(0);
		DOF_Sampler_Focal_HBlur_Focal->Set(0);
		DOF_Sampler_Focal_VBlur_Focal->Set(0);
		DOF_Sampler_Shading_HBlur_Focal->Set(0);
		DOF_Sampler_Shading_HBlur_Shading->Set(1);
		DOF_Sampler_Shading_VBlur_Focal->Set(0);
		DOF_Sampler_Shading_VBlur_Shading->Set(1);
		
		DOF_Sampler_DepthOfField_Color->Set(0);
		DOF_Sampler_DepthOfField_Blur->Set(1);
		DOF_Sampler_DepthOfField_Focal->Set(2);
		
}

void DepthOfField::createBuffers()
{
	
		DOF_FBO_Focal 			= new GPUFBO("DOF_FBO_Focal");
		DOF_FBO_Focal_Blur[0]	= new GPUFBO("DOF_FBO_Focal_HBlur");
		DOF_FBO_Focal_Blur[1]	= new GPUFBO("DOF_FBO_Focal_VBlur");
		DOF_FBO_Shading_Blur[0]	= new GPUFBO("DOF_FBO_Shading_HBlur");
		DOF_FBO_Shading_Blur[1]	= new GPUFBO("DOF_FBO_Shading_VBlur");
		


		DOF_FBO_Focal->createTexture2DAttachments(			fbo_width, fbo_height, 1, false, GL_R16F);
		DOF_FBO_Focal_Blur[0]->createTexture2DAttachments(	fbo_width, fbo_height, 1, false, GL_R16F);
		DOF_FBO_Focal_Blur[1]->createTexture2DAttachments(	fbo_width, fbo_height, 1, false, GL_R16F);
		DOF_FBO_Shading_Blur[0]->createTexture2DAttachments(fbo_width, fbo_height, 1, false, GL_RGB16F);
		DOF_FBO_Shading_Blur[1]->createTexture2DAttachments(fbo_width, fbo_height, 1, false, GL_RGB16F);
		
}

void DepthOfField::createParameters()
{
	
	dofCameraNear	= DOF_PRG_Focal_Depth->uniforms()->getGPUfloat("zNear");
		dofCameraFar 	= DOF_PRG_Focal_Depth->uniforms()->getGPUfloat("zFar");
		if(useNearDOF && useFarDOF)
		{
			dofFocalNear 	= DOF_PRG_Focal_Depth->uniforms()->getGPUfloat("FocalNear");
			dofFocalFar		= DOF_PRG_Focal_Depth->uniforms()->getGPUfloat("FocalFar");
			dofFocalNear->Set(nearFocal);
			dofFocalFar->Set(farFocal);
		}
		else
		{
			if(useNearDOF)
			{
				dofFocalNear 	= DOF_PRG_Focal_Depth->uniforms()->getGPUfloat("FocalNear");
				dofFocalNear->Set(nearFocal);
			}
			if(useFarDOF)
			{
				dofFocalFar		= DOF_PRG_Focal_Depth->uniforms()->getGPUfloat("FocalFar");
				dofFocalFar->Set(farFocal);
			}
		}		
		dofFocalHBlurSize			= DOF_PRG_Focal_HBlur->uniforms()->getGPUint("dofFocalBlurSize");
		dofFocalHBlurHStep			= DOF_PRG_Focal_HBlur->uniforms()->getGPUfloat("HStep");
		dofFocalVBlurSize			= DOF_PRG_Focal_VBlur->uniforms()->getGPUint("dofFocalBlurSize");
		dofFocalVBlurVStep			= DOF_PRG_Focal_VBlur->uniforms()->getGPUfloat("VStep");	
		dofShadingHBlurSize			= DOF_PRG_Shading_HBlur->uniforms()->getGPUint("dofShadingBlurSize");
		dofShadingHBlurHStep		= DOF_PRG_Shading_HBlur->uniforms()->getGPUfloat("HStep");
		dofShadingVBlurSize			= DOF_PRG_Shading_VBlur->uniforms()->getGPUint("dofShadingBlurSize");
		dofShadingVBlurVStep		= DOF_PRG_Shading_VBlur->uniforms()->getGPUfloat("VStep");

		dofCameraNear->Set(Scene::getInstance()->camera()->getZnear());
		dofCameraFar->Set(Scene::getInstance()->camera()->getZfar());
		dofFocalHBlurSize->Set(focalBlurSize);
		dofFocalHBlurHStep->Set(1.0f / DOF_FBO_Focal_Blur[0]->getWidth());
		dofFocalVBlurSize->Set(focalBlurSize);
		dofFocalVBlurVStep->Set(1.0f / DOF_FBO_Focal_Blur[1]->getHeight());
		dofShadingHBlurSize->Set(shadingBlurSize);
		dofShadingHBlurHStep->Set(1.0f / DOF_FBO_Shading_Blur[0]->getWidth());
		dofShadingVBlurSize->Set(shadingBlurSize);
		dofShadingVBlurVStep->Set(1.0f / DOF_FBO_Shading_Blur[1]->getHeight());
		
}


DepthOfField::~DepthOfField()
{
	
	delete Base;
	

	
}



void DepthOfField::apply(GPUFBO *Depth, GPUFBO* Shading, GPUFBO* out)
{
	
m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Focal_Depth);

	DOF_FBO_Focal->enable();
		Depth->getColorTexture(0)->bind(DOF_Sampler_Focal_Depth_Depth->getValue());
			this->drawQuad();
		Depth->getColorTexture(0)->release();
	DOF_FBO_Focal->disable();

	//----- Focal Blur Pass -----//
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Focal_HBlur);

	DOF_FBO_Focal_Blur[0]->enable();
		DOF_FBO_Focal->getColorTexture(0)->bind(DOF_Sampler_Focal_HBlur_Focal->getValue());
			this->drawQuad();
		DOF_FBO_Focal->getColorTexture(0)->release();
	DOF_FBO_Focal_Blur[0]->disable();

	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Focal_VBlur);

	DOF_FBO_Focal_Blur[1]->enable();
		DOF_FBO_Focal_Blur[0]->getColorTexture(0)->bind(DOF_Sampler_Focal_VBlur_Focal->getValue());
			this->drawQuad();
		DOF_FBO_Focal_Blur[0]->getColorTexture(0)->release();
	DOF_FBO_Focal_Blur[1]->disable();

	//----- Shading Blur Pass -----//
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Shading_HBlur);

	DOF_FBO_Shading_Blur[0]->enable();
		Shading->getColorTexture(0)->bind(DOF_Sampler_Shading_HBlur_Shading->getValue());
		DOF_FBO_Focal_Blur[1]->getColorTexture(0)->bind(DOF_Sampler_Shading_HBlur_Focal->getValue());
			this->drawQuad();
		DOF_FBO_Focal_Blur[1]->getColorTexture(0)->release();
		Shading->getColorTexture(0)->release();
	DOF_FBO_Shading_Blur[0]->disable();

	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_Shading_VBlur);

	DOF_FBO_Shading_Blur[1]->enable();
		DOF_FBO_Shading_Blur[0]->getColorTexture(0)->bind(DOF_Sampler_Shading_VBlur_Shading->getValue());
		DOF_FBO_Focal_Blur[1]->getColorTexture(0)->bind(DOF_Sampler_Shading_VBlur_Focal->getValue());
			this->drawQuad();
		DOF_FBO_Focal_Blur[1]->getColorTexture(0)->release();
		DOF_FBO_Shading_Blur[0]->getColorTexture(0)->release();
	DOF_FBO_Shading_Blur[1]->disable();

	//----- Depth of Field -----//
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DOF_PRG_DepthOfField);

	if (out != NULL)
		out->enable();
		
		Shading->getColorTexture(0)->bind(DOF_Sampler_DepthOfField_Color->getValue());
		DOF_FBO_Focal_Blur[1]->getColorTexture(0)->bind(DOF_Sampler_DepthOfField_Focal->getValue());
		DOF_FBO_Shading_Blur[1]->getColorTexture(0)->bind(DOF_Sampler_DepthOfField_Blur->getValue());
			this->drawQuad();
		DOF_FBO_Shading_Blur[1]->getColorTexture(0)->release();
		DOF_FBO_Focal_Blur[1]->getColorTexture(0)->release();
		Shading->getColorTexture(0)->release();

	if (out != NULL)
		out->disable();
	

}

