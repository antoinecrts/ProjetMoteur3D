#include "TPMaterial.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Scene.h"


TPMaterial::TPMaterial(std::string name):
	MaterialGL(name,"TPMaterial")
{
	/* Matrice de projection du model  */
	ModelM = vp->uniforms()->getGPUmat4("Model");
	modelViewProj = vp->uniforms()->getGPUmat4("MVP");
	/* Position du modèle Particules */
	PartPos = vp->uniforms()->getGPUvec3("PartPos");
	/* Position de la caméra  */
	CamPos = vp->uniforms()->getGPUvec3("CamPos");

	/* Chargement des textures */

	/* Texture : diffuse original */
	text = new GPUTexture2D(ressourceTexPath + "texture_03_diffuseOriginal.png");
	/* La texture contenant les normales */
	text_n = new GPUTexture2D(ressourceTexPath + "texture_03_normal.png");
	/* Texture : height */
	text2 = new GPUTexture2D(ressourceTexPath + "texture_03_height.png");
	/* Texture : ao */
	text3 = new GPUTexture2D(ressourceTexPath + "texture_03_ao.png");
	/* Texture : roughness */
	text4 = new GPUTexture2D(ressourceTexPath + "texture_03_roughness.png");

	///////////////////////////////////////////
	//* Utilisation dans le fragment shader *//
	///////////////////////////////////////////

	difftext = fp->uniforms()->getGPUsampler("difftext");
	difftext->Set(text->getHandle());

	normtext = fp->uniforms()->getGPUsampler("normtext");
	normtext->Set(text_n->getHandle());

	heitext = fp->uniforms()->getGPUsampler("heitext");
	heitext->Set(text2->getHandle());

	aotext = fp->uniforms()->getGPUsampler("aotext");
	aotext->Set(text3->getHandle());

	roughtext = fp->uniforms()->getGPUsampler("roughtext");
	roughtext->Set(text4->getHandle());


}
TPMaterial::~TPMaterial()
{

}

void TPMaterial::setColor(glm::vec4 &c)
{
	
}

void TPMaterial::render(Node *o)
{

	modelViewProj->Set(o->frame()->getTransformMatrix());
	
	if (m_ProgramPipeline)
	{		
		m_ProgramPipeline->bind();
		o->drawGeometry(GL_TRIANGLES);
		m_ProgramPipeline->release();
	}
}

void TPMaterial::update(Node* o,const int elapsedTime)
{

	/* La source de lumière sera l'objet Particules du projet */
	/* On place alors un point afin que les particules éclairent l'objet central  */

	/* Position de la source de lumiere dans le repere de la scene */
	glm::vec3 Point = glm::vec3(1.2,-0.5, 0.0);

	/* Repere de l'objet */
	Frame* dest = o->frame();
	/* Exprime la position de la source lumineuse dans le repère de l'objet  */
	PartPos->Set(Scene::getInstance()->frame()->convertPtTo(Point, dest));
	
	/* Position de la caméra dans le repère de la scène */
	glm::vec3 Point2 = glm::vec3(0.0, 0.0, 0.0);
	/* Position de la caméra dans le repère de l'objet */
	CamPos->Set(Scene::getInstance()->camera()->convertPtTo(Point2, dest));


}
