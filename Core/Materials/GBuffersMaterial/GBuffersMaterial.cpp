#include "GBuffersMaterial.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Scene.h"


GBuffersMaterial::GBuffersMaterial(std::string name):
	MaterialGL(name,"GBuffersMaterial")
{
	this->vs_NormalM = vp->uniforms()->getGPUmat4("NormalM");
	this->vs_M = vp->uniforms()->getGPUmat4("M");
	this->modelViewProj = vp->uniforms()->getGPUmat4("MVP");

	lm = NULL;

	 lm_sampler = fp->uniforms()->getGPUsampler("lightmap");

}
GBuffersMaterial::~GBuffersMaterial()
{

}
void GBuffersMaterial::updateCamera()
{

}

void GBuffersMaterial::render(Node *o)
{

	update(o, 0.0);
	if (m_ProgramPipeline)
	{
	
		m_ProgramPipeline->bind();
		//o->drawGeometry(GL_PATCHES);
		o->drawGeometry(GL_TRIANGLES);
		m_ProgramPipeline->release();
	}
}
void GBuffersMaterial::update(Node* o, const int elapsedTime)
{


	glm::mat4 M = o->frame()->getRootMatrix();
	glm::mat4 NormalM = glm::transpose(glm::inverse(M));


	if (o->frame()->updateNeeded())
	{
		vs_M->Set(M);
		vs_NormalM->Set(NormalM);
	}
}
void GBuffersMaterial::setLightTexture(GPUTexture2D* _lm_text)
{

	lm = _lm_text;
	fp->uniforms()->getGPUsampler("lightmap")->Set(lm->getHandle());
}

