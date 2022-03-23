
#include "DeferredEngineGL.h"
#include "deferredSceneLoaderGL.h"
#include "Materials/DeferredMaterial/DeferredMaterial.h"

DeferredEngineGL::DeferredEngineGL (int width, int height) :
EngineGL (width, height)
{
	GeometryPassFBO = NULL;
	PostProcessFBO = NULL;

	initGeometryFBO();
	
}

DeferredEngineGL::~DeferredEngineGL ()
{
}


bool DeferredEngineGL::init (std::string filename)
{
	

	// Load Scene information here
	if (filename.size() > 0)
	{
		deferredSceneLoaderGL* sceneloader = new deferredSceneLoaderGL();
		Node* sceneLoaded = sceneloader->loadScene(filename);
		sceneLoaded->frame()->scale(glm::vec3(0.082));
		sceneLoaded->frame()->rotate(glm::vec3(0, 1, 0), 3.1415926535f*0.5f);
		scene->getSceneNode()->adopt(sceneLoaded);
		delete sceneloader;

	}
	LightMaterial* lmat = new LightMaterial("LightMaterial-" + string("DefaultLight"), glm::vec4(1));

	glm::vec4 sun = glm::vec4(1, 1, 0.6, 1);

	addLight("MyLight", glm::vec3(0.0, 0.0, 30.0),  glm::vec4(0.7, 0.7, 1, 1));
	addLight(string("SunLight"),  glm::vec3(-160, 120.0, 160),  sun);
	addLight("RedLight",  glm::vec3(180, 70.0, 0),  glm::vec4(0.8, 0.3, 0.3, 1));
	PostProcessFBO = scene->getResource<GPUFBO>("PostProcessing FBO");
	PostProcessFBO->createTexture2DAttachments(FBO_WIDTH, FBO_HEIGHT, 1, false, GL_RGB16F);
	postProcessing = scene->getEffect<PostProcess>("PostProcessing");
	
	def = new DeferredMaterial("DeferredMaterial");
	setUpEngine();
	return(true);
}


void DeferredEngineGL::deferredRender()
{
	GeometryPassFBO->enable();

		glDepthMask (GL_TRUE);
		glClearColor (0.0, 0.0, 0.0, 1.0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable (GL_DEPTH_TEST);
			//
		GeometryPassFBO->drawBuffers(6);
			//
		////Rendering every collected node
		def->m_ProgramPipeline->bind();
		for (unsigned int i = 0; i < renderedNodes->nodes.size (); i++)
			renderedNodes->nodes[i]->render();
		def->m_ProgramPipeline->release();
		glDisable (GL_DEPTH_TEST);
		glDepthMask (GL_FALSE);
		GeometryPassFBO->disable();
}



void DeferredEngineGL::render ()
{
	
	// Begin Time query
	timeQuery->begin ();
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// -- Deferred rendering pass
		
	deferredRender();


	// Application du PostProcess
	postProcessing->apply(GeometryPassFBO, PostProcessFBO);

// Affichage du FBO contenant l'image finale
	PostProcessFBO->display();
		
	drawBBAndLight();

	// end time Query					
	timeQuery->end ();


	scene->needupdate = false;

}

void DeferredEngineGL::animate (const int elapsedTime)
{
	
	// Animate each node
	for (unsigned int i = 0; i < allNodes->nodes.size (); i++)
		allNodes->nodes[i]->animate (elapsedTime);


	// force update of lighting model
	lightingModel->update (true);

}




void DeferredEngineGL::initGeometryFBO()
{

		
	GeometryPassFBO = scene->getResource<GPUFBO>("Geometry Pass");
	GeometryPassFBO->createTexture2DArrayAttachment(FBO_WIDTH, FBO_HEIGHT, 7, true, GL_RGB16F);

}
