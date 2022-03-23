#include "MotionBlur.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"



MotionBlur::MotionBlur(std::string name) :
	EffectGL(name,"PostProcess/MotionBlur")
{


	Base = new GLProgram(this->m_ClassName + "-Base", GL_VERTEX_SHADER);
	m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT, Base);
	MB_PRG_MotionBlur = new GLProgram (this->m_ClassName + "-MB-MotionBlur-PerObject", GL_FRAGMENT_SHADER);
	m_ProgramPipeline->useProgramStage (GL_FRAGMENT_SHADER_BIT, MB_PRG_MotionBlur);
	m_ProgramPipeline->link ();

	mbTime = 0.0f;
	mbLastTime = 0.0f;


	createSamplers();
	


}


void MotionBlur::createSamplers()
{
		MB_Sampler_MotionBlur_Color = MB_PRG_MotionBlur->uniforms ()->getGPUsampler ("ColorSampler");
		MB_Sampler_MotionBlur_Color->Set (0);
		MB_Sampler_MotionBlur_Velocity = MB_PRG_MotionBlur->uniforms ()->getGPUsampler ("GBuffers");
		MB_Sampler_MotionBlur_Velocity->Set (1);

		velocityScale = MB_PRG_MotionBlur->uniforms ()->getGPUfloat ("velocityScale");
		velocityScale->Set (1.0f);
		
}


MotionBlur::~MotionBlur()
{
	
	delete Base;
	

	
}



void MotionBlur::apply(GPUFBO *GBuffers, GPUFBO* Shading, GPUFBO* out)
{
	auto now = std::chrono::high_resolution_clock::now();
	float delta = std::chrono::duration_cast< std::chrono::duration<long, std::milli > > (now - lastUpdateTime).count () * 0.001;
	lastUpdateTime = now;

	float FPS = 1 / ( delta );
	float lastFrameFPS = 1 / ( lastDelta );
	lastDelta = delta;

	float timeForMB = 0.9 * lastFrameFPS + 0.1 * FPS;

	velocityScale->Set (timeForMB*0.01666); // 0.016 = 1 / 60 => FPS Target

	
	m_ProgramPipeline->useProgramStage (GL_FRAGMENT_SHADER_BIT, MB_PRG_MotionBlur);
	
	if (out != NULL)
		out->enable ();
	Shading->getColorTexture (0)->bind (MB_Sampler_MotionBlur_Color->getValue ());
	GBuffers->getColorTexture (0)->bind (MB_Sampler_MotionBlur_Velocity->getValue ());
	this->drawQuad ();
	Shading->getColorTexture (0)->release ();
	GBuffers->getColorTexture (0)->release ();
	if (out != NULL)
		out->disable ();

	

}

