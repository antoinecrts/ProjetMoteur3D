/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet
 *
 */
#include <string>
#include "EngineGL.h"
#include "Engine/OpenGL/Lighting/LightingModelGL.h"
#include "SceneLoaderGL.h"
#include "Engine/Base/NodeCollectors/MeshNodeCollector.h"
#include "Engine/Base/NodeCollectors/FCCollector.h"



EngineGL::EngineGL(int width, int height):
Engine()
{
	this->scene = Scene::getInstance();
	this->allNodes = new StandardCollector();

	this->renderedNodes = new MeshNodeCollector();
	w_Width = width;
	w_Height = height;
	this->lightingModel = NULL;	
	
	timeQuery = new GPUQuery("Timer",GL_TIME_ELAPSED);
	scene->needupdate = true;	
	drawLights = false;
	drawBoundingBoxes = false;

	LOG_INFO << "Initializing Scene" << std::endl;
	// timeQuery->create ();

	LOG_INFO << GPUInfo::Instance()->getOpenGLVersion() << std::endl;

	// Load shaders in "\common" into graphic cards
	GLProgram::prgMgr.addPath(ressourceMaterialPath + "Common", "");	
	GLProgram::prgMgr.addPath(ressourceCoreMaterialPath + "Common", "");


		// Create Bounding Box Material for bounding box rendering
	boundingBoxMat = new BoundingBoxMaterial ("BoundingBoxMat");

	glState = EngineGLState::getInstance();

	// OpenGL state variable initialisation
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glState->setViewport(glm::ivec4(0, 0, width, height));
	glState->pushViewport();
	// Create Lighting Model GL and collect all light nodes of the scene 
	lightingModel = new LightingModelGL("LightingModel", scene->getRoot());
}

EngineGL::~EngineGL()
{
	if (allNodes != NULL)
	{
		delete allNodes;
		allNodes = NULL;
	}
	if (renderedNodes != NULL)
	{
		delete renderedNodes;
		renderedNodes = NULL;
	}
} 

bool EngineGL::init(std::string filename)
{
	
	// Loading a scene or a mesh and add it to the root of the Engine scenegraphe

	if (!loadScene(filename))
		loadScene(ressourceCoreObjPath + "Sphere.obj");

	setUpEngine();
	return(true);
}
bool EngineGL::loadScene(std::string filename)
{
	SceneLoaderGL* sceneloader = new SceneLoaderGL();
	Node* sceneLoaded = sceneloader->loadScene(filename);
	if (sceneLoaded != NULL)
		scene->getSceneNode()->adopt(sceneLoaded);

	delete sceneloader;

	return (sceneLoaded != NULL);
}
void EngineGL::setUpEngine()
{
	
	// Force window Resize
	this->onWindowResize(w_Width, w_Height);

	// Bind the light buffer for rendering
	lightingModel->bind();

	allNodes->collect(scene->getRoot());

	renderedNodes->collect(scene->getRoot());

	for (unsigned int i = 0; i < allNodes->nodes.size(); i++)
		allNodes->nodes[i]->animate(0);

	Scene::getInstance()->camera()->updateBuffer();
}

void EngineGL::render()
{
		// Begin Time query
		// timeQuery->begin();

		// Clear Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rendering every collected node
		for (unsigned int i = 0; i < renderedNodes->nodes.size(); i++) 
			renderedNodes->nodes[i]->render();
		// end time Query					
		// timeQuery->end();

		if (drawLights)
			lightingModel->renderLights();

		if (drawBoundingBoxes)
			for (unsigned int i = 0; i < renderedNodes->nodes.size(); i++) 
				renderedNodes->nodes[i]->render(boundingBoxMat);

		// scene has been rendered : no need for recomputing values until next camera/parameter change
		//Scene::getInstance()->camera()->setUpdate(true);
		scene->needupdate = false;	
}

void EngineGL::animate(const int elapsedTime)
{
	//dynamic_cast<FCCollector*> (renderedNodes)->updateCam();
	// Collect all relevant nodes
	//renderedNodes->collect(scene->getRoot());
	// Animate each node
	for (unsigned int i = 0; i < allNodes->nodes.size(); i++)
		allNodes->nodes[i]->animate(elapsedTime);
	// force update of lighting model
	Scene::getInstance()->camera()->updateBuffer();
	lightingModel->update(true); 
}

void EngineGL::onWindowResize(int width, int height)
{
	
	glState->setViewport(glm::ivec4(0, 0, width, height));
	scene->camera()->setAspectRatio((float)width / (float)height);
	w_Width = width;
	w_Height = height;

	if (lightingModel != NULL)
		lightingModel->setWindowSize(glm::vec2(width, height));
		
}

void EngineGL::setWidth(int w)
{
	w_Width = w;
}
void EngineGL::setHeight(int h)
{
	w_Height = h;
}

int EngineGL::getWidth()
{
	return w_Width;
}

int EngineGL::getHeight()
{
	return w_Height;
}
double EngineGL::getFrameTime()
{
	double frame_time = timeQuery->getResultUInt();
	//double frame_time = 5;
	frame_time = frame_time*0.000001;
	return frame_time;
}

void EngineGL::addObject(string name, string path,Material* m, glm::vec3 scale, glm::vec3 translate, glm::vec4 rotate) {
	Node* sceneLoaded = Scene::getInstance()->getNode(name);
	ModelGL *baseModel = Scene::getInstance()->getModel<ModelGL>(path);

	sceneLoaded->frame()->translate(translate);
	sceneLoaded->frame()->scale(scale);

	if (rotate.x != 0 || rotate.y != 0 || (rotate.z != 0 && rotate.w != 0))
		sceneLoaded->frame()->rotate(glm::vec3(rotate), rotate.w);

	sceneLoaded->setModel(baseModel);
	sceneLoaded->setMaterial(m, true);
	Scene::getInstance()->getSceneNode()->adopt(sceneLoaded);
}
void EngineGL::addLight(string name, glm::vec3 position,  glm::vec4 color,int type, LightMaterial* lMat) 
{
	LOG_TRACE << "Adding Light : " << name << std::endl;
	LightNode *lnode = Scene::getInstance()->getNodes<LightNode>(name);


	lnode->setModel(Scene::getInstance()->getModel<ModelGL>(ressourceCoreObjPath + "Sphere.obj"));

	if (lMat == NULL)
		lMat =  new LightMaterial ("LightMaterial-" + name, color);


	lnode->setMaterial(lMat);

	Scene::getInstance()->getSceneNode()->adopt(lnode);

	lnode->frame()->translate(position);
	lnode->frame()->scale(glm::vec3(20.0, 20.0, 20.0));
	lnode->setPosition(glm::vec4(position, 0.0));
	lnode->setType(type);
	lnode->setColor(color);
	lightingModel->collect();
}

void EngineGL::drawBBAndLight()
{
	if (drawLights)
		lightingModel->renderLights();

	if (drawBoundingBoxes)
		for (unsigned int i = 0; i < renderedNodes->nodes.size(); i++)
			renderedNodes->nodes[i]->render(boundingBoxMat);
}
void EngineGL::setClearColor(glm::vec4 color)
{
	glClearColor(color.x,color.y,color.z,color.w);
}

void EngineGL::displayInterface()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Lighting"))
		{
			ImGui::MenuItem("Lighting Model", NULL, &(lightingModel->show_interface));		
			ImGui::EndMenu();
		}
		if (lightingModel->show_interface)
			lightingModel->displayInterface(lightingModel->getName().c_str());
		ImGui::EndMainMenuBar();
	}

}