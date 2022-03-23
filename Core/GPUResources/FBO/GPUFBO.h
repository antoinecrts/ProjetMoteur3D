#ifndef _GPUFBO_
#define _GPUFBO_

#include <iostream>
#include <GL/glew.h>
#include "Engine/Base/Resource.h"
#include "Engine/OpenGL/ModelGL.h"
#include "Engine/Base/Scene.h"
#include "GPUResources/Textures/GPUTexture2D.h"
#include "GPUResources/Textures/GPUTexture2DArray.h"
#include "Engine/OpenGL/EngineGLState.h"
class Material;
class DisplayResource;
class DisplayLayer;
// TODO : heritage : GPUFBO1D , 1DARRAY, 2D, 2DARRAY, 3D


class GPUFBO : public Resource
{
public:
	GPUFBO(std::string name);
	~GPUFBO();

	bool createTexture2DAttachments(int w=512,int h=512, int numberOfTarget=1,bool useDepth=true,GLint format = GL_RGBA8);
	
	bool createTextureArray(int w=512,int h=512,int layerDepth = 1,  int numberOfTarget=1,bool useDepth=true,GLint format = GL_RGBA8);

	

	void createTexture2DAttachment(int w=512,int h=512,GLint format= GL_RGBA8);
	void createTexture2DArrayAttachment(int w=512,int h=512,int l=512,bool useDepth=true,GLint format= GL_RGBA8);
	bool createTexture3DAttachment(int w=512,int h=512, int d=512,bool useDepth=true,GLint format = GL_RGBA8);


	void createDepthAttachment(int w,int h);
	void bindTextureToBuffer(int target,int attachment, int level=0);
	void bindLayerToBuffer(int target, int layer,int level=0);


	void enable();
	void enable(int layer,int attachment = 0);
	void disable();




void destroy();
	

	void bindColorTexture(int channel = 0,int layer = 0);
	
	void releaseColorTexture();
	void bindDepthTexture(int channel = 0);
	void releaseDepthTexture();

	unsigned int getDepthBuffer();
	GPUTexture* getColorTexture(int layer =0);
	GPUTexture* getDepthTexture();
	void setDepthBuffer(GPUTexture* de);
	void drawBuffer(int i);
	void drawBuffers(int i);

	int getWidth();
	int getHeight();

	bool isInitialized();

	int layer_bound;

	bool* bufferIsBound;

	virtual void display(const glm::vec4  &box = glm::vec4(0.0,0.0,1.0,1.0),int layer = 0);
	virtual void displayDepth(const glm::vec4  &box = glm::vec4(0.0,0.0,1.0,1.0));

	GLuint getFBOid();

	void switchToTexture2DArray();

	void changeResolution(glm::ivec2 newRes);

	void writeToFile(string filename);
	virtual void displayInterface();
	

private:
	int m_width;
	int m_height;	
	int m_depth;

	int m_number;
	std::string info_text;
	bool m_use_depth;
	GLint m_format;
	GLuint m_FBOId;
	bool CheckFramebufferStatus();

	Scene *scene;

	bool initialized;
	DisplayResource *HUD;
	DisplayLayer*	HUDArray;

	GLuint m_Target;

	std::vector<GPUTexture*> colorBuffers;

	GPUTexture2DArray*		colorBufferArray;

	GPUTexture* depthBuffer;

	int oldFB;

	EngineGLState* glState;
};


#endif
