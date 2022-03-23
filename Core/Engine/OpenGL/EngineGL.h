/**
 *
 * @author     G.Gilet, Guillaume.Gilet@unilim.fr
 * @date       20 August 2017
 * @version    1.0 
 */

#ifndef _ENGINE_GL_H
#define _ENGINE_GL_H

#include <map>
#include <string>
#include <stack>
#include <GL/glew.h>
#include "Engine/Base/Engine.h"
#include "Engine/Base/NodeCollectors/StandardCollector.h"

#include "Materials/LightMaterial/LightMaterial.h"
#include "GPUResources/Query/GPUQuery.h"
#include "Engine/OpenGL/Lighting/LightingModelGL.h"
#include "Materials/BoundingBoxMaterial/BoundingBoxMaterial.h"

#include "GPUResources/GPUInfo.h"
#include "EngineGLState.h"

/**
 * @brief      This class is the parent class for creating an forward rendering engine using OpenGL
 *
 * @details    Each project should have its own engine which inherit this parent engine
 *
 */

class EngineGL : public Engine
{
	public:
	/**
	 * @brief	Creates an forward rendering engine
	 * @details Creates an forward rendering engine with predefined cameraGL, timers objects and a LightingModelGL
	 * @param	width as rendering frame width
	 * @param	height as rendering frame height		
	 */
	EngineGL(int width, int height); 
	/**
	 * @brief      Destroy an engine
	 */

	~EngineGL();
	/**
	 * @brief	Initialize the scene and various objects (node, models, materials, effects, FBO...) used by the engine
	 * @param	filename of a target scene in a known format (.DAE,.3DS,.OBJ)
	 * @return success of the initialization		
	 */

	virtual bool init(std::string filename="");
	/**
	 * @brief	Load a Scene according to the provided filename
	 * @param	filename of a target scene in a known format (.DAE,.3DS,.OBJ)
	 * @return success of the loading operation		
	 */
	virtual bool loadScene(std::string filename);
	/**
	 * @brief	Render the scene. In the parent class, this is a forward rendering of each node of the scene
	 */
	virtual void render();
	/**
	 * @brief	Animate the scene. This function should provide update for camera movement, node animation (by calling the animate function of each node) and update the lighting and common buffers. 
	 */
	virtual void animate(const int elapsedTime);

	/**
	 * @brief	Resize the viewport
	 * @param	w as width of the viewport
	 * @param	h as height of the viewport	
	 */

	virtual void onWindowResize(int w,int h);
	/**
	 * @brief	Get Time stamp of the last rendered frame
	 * @return timestamp of the last rendered frame
	 */
	double getFrameTime();
	/**
	 * @brief	Get width of the current frame
	 * @return width of the frame
	 */
	int getWidth();
	/**
	 * @brief	Get height of the current frame
	 * @return height of the frame
	 */
	int getHeight();

	void setWidth(int w); 
	/**
	 * @brief	Set width of the current frame
	 * @param w width of the frame
	 */
	void setHeight(int h);
	/**
	 * @brief	Set height of the current frame
	 * @param h height of the frame
	 */

	bool drawLights;
	bool drawBoundingBoxes;

	/**
	 * @brief	Add a light to the scene. Helper function to quickly add lights to the scene
	 * @details Create a LighNode and add it to the top node of the scene
	 * @param name name of the light
	 * @param position position of the light
	 * @param color color of the light
	 * @param type type of the light (not available for now : use SPOT_LIGHT)
	 * @param lMat a valid Material for rendering the light. If NULL , create a standard lightMaterial 
	 */

	void addLight(string name, glm::vec3 position,  glm::vec4 color = glm::vec4(1.0),int type = SPOT_LIGHT, LightMaterial* lMat = NULL);
		/**
	 * @brief	Add a Object to the scene. Helper function to quickly add obects to the scene
	 * @details Create a Node, load its geometricModelGL with a valid object defined by its filename, and add it to the top node of the scene
	 * @param name name of the node 
	 * @param path name of the file containing the geometric model for this node
	 * @param m a valid material for rendering the node
	 * @param scale scale operation applid to the node frame	 
	 * @param translate translate operation applid to the node frame
	 * @param rotate rotate operation applid to the node frame
	 */
	void addObject(string name, string path, Material* m, glm::vec3 scale = glm::vec3(1.0), glm::vec3 translate = glm::vec3(0.0), glm::vec4 rotate = glm::vec4(0.0));
	/**
	 * @brief	Draw Bounding Boxes and lights. Not in use for now
	 */
	void drawBBAndLight();
	/**
	 * @brief	Define the background color
	 * @param color the background color
	 */
	void setClearColor(glm::vec4 color);
	/**
	 * @brief	Prepare the engine for rendering. Should be called at the end of init
	 * @details Set up the engine. Parse nodes using standard collectors, bind and populates the lighting model with LightNodes
	 */	
	virtual void setUpEngine();
	/**
	 * @brief	Display interface for the engine (using imgui)
	 */	
	virtual void displayInterface();

protected:
	int w_Width;
	int w_Height;

	NodeCollector *allNodes;
	NodeCollector *renderedNodes;

	LightingModelGL *lightingModel;
	BoundingBoxMaterial *boundingBoxMat;
	 
	GPUQuery* timeQuery;

	EngineGLState* glState;
	

};
#endif
