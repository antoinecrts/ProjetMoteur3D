
#include "Blur.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"



Blur::Blur(std::string name):
	EffectGL(name,"Blur")
{
	/* Vertex shader */
	vp = new GLProgram(this->m_ClassName+"-Base",GL_VERTEX_SHADER);
	/* Fragment Shader */
	fp = new GLProgram(this->m_ClassName + "-Pass", GL_FRAGMENT_SHADER);
	
	m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT,vp);
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, fp);
	m_ProgramPipeline->link();
	
	/* On utilise la texture dans le fragment shader */
	textsampler = fp->uniforms()->getGPUsampler("fboIn");
	textsampler->Set(0);

}
Blur::~Blur()
{
	
	delete vp;
}


void Blur::apply(GPUFBO *in)
{
	
	glDisable(GL_DEPTH_TEST);
	if (m_ProgramPipeline)
	{
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT,fp);
		in->enable();
			in->bindColorTexture(0);
			m_ProgramPipeline->bind();
			quad->drawGeometry(GL_TRIANGLES);
			m_ProgramPipeline->release();
			in->releaseColorTexture();
		in->disable();
	}

	glEnable(GL_DEPTH_TEST);


}
void Blur::displayInterface()
{

}

