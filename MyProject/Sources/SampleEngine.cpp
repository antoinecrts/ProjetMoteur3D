/*
EngineGL overloaded for custom rendering
*/
#include "SampleEngine.h"
#include "Engine/OpenGL/v4/GLProgram.h"
#include "Engine/OpenGL/SceneLoaderGL.h"
#include "Engine/Base/NodeCollectors/MeshNodeCollector.h"
#include "Engine/Base/NodeCollectors/FCCollector.h"

#include "Materials/TPMaterial/TPMaterial.h"

#include "GPUResources/GPUInfo.h"


SampleEngine::SampleEngine(int width, int height) :
EngineGL (width, height)
{
	LOG_INFO << "# - " << __FUNCTION__ << std::endl;
	
	
}

SampleEngine::~SampleEngine()
{


}


bool SampleEngine::init(std::string filename)
{
	//Nos matériaux

	TPMaterial* material = new TPMaterial("TPMaterial");

	LightMaterial* material_lum = new LightMaterial("LightMaterial");

	//////////////////////////////////
	// L'objet central : la planète //
	//////////////////////////////////

	Node* objet = scene->getNode("objet");
	objet->setModel(scene->m_Models.get<ModelGL>(ressourceCoreObjPath + "Planet.obj"));
	objet->setMaterial(material);
	scene->getSceneNode()->adopt(objet);

	
	///////////////////////////////////////
	// Des ajouts d'objets dans la scene //
	///////////////////////////////////////
	
	/* Notre simulation de particules */

	Node* part = scene->getNode("part");
	part->setModel(scene->m_Models.get<ModelGL>(ressourceCoreObjPath + "Particules.obj"));
	part->frame()->translate(glm::vec3(1.2,-0.2, 0.0));
	part->frame()->scale(glm::vec3(10.0));
	part->setMaterial(material_lum);
	scene->getSceneNode()->adopt(part);
	
	Node* part2 = scene->getNode("part2");
	part2->setModel(scene->m_Models.get<ModelGL>(ressourceCoreObjPath + "Particules.obj"));
	part2->frame()->translate(glm::vec3(1.2,-0.2, -0.5));
	part2->frame()->scale(glm::vec3(15.0));
	part2->setMaterial(material_lum);
	scene->getSceneNode()->adopt(part2);

	Node* part3 = scene->getNode("part3");
	part3->setModel(scene->m_Models.get<ModelGL>(ressourceCoreObjPath + "Particules.obj"));
	part3->frame()->translate(glm::vec3(1.2, -0.2, 0.6));
	part3->frame()->scale(glm::vec3(15.0));
	part3->setMaterial(material_lum);
	scene->getSceneNode()->adopt(part3);

	/* Couleur du fond */
	glClearColor(0.11,0.10,0.10,1.0); 

	/* Fragment Buffer Object */
	monFBO = scene->getResource<GPUFBO>("monFBO");
	monFBO->createTexture2DAttachments(2048, 2048);

	/* Effet de flou */
	monBLUR = Scene::getInstance()->getEffect<Blur>("Blur");


	setUpEngine();
	LOG_INFO << "initialisation complete" << std::endl;
	return(true);
}


void SampleEngine::render ()
{
	/* Activation du FBO */
	monFBO->enable();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (unsigned int i = 0; i < renderedNodes->nodes.size(); i++)
		renderedNodes->nodes[i]->render();

	drawBBAndLight();

	/* Desactivation du FBO */
	monFBO->disable();

	/* On nettoie le FBO */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Application du filtre sur le FBO */
	monBLUR->apply(monFBO);

	/* On l'affiche */
	monFBO->display();


}

void SampleEngine::animate (const int elapsedTime)
{
	// Animate each node
	for (unsigned int i = 0; i < allNodes->nodes.size (); i++)
		allNodes->nodes[i]->animate (elapsedTime);
	// force update of lighting model
	lightingModel->update (true);
	Scene::getInstance()->camera()->updateBuffer();
}

