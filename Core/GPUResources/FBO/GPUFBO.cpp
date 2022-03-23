#include <sstream>
#include "GPUFBO.h"
#include "Engine/OpenGL/EngineGL.h"
#include "Engine/OpenGL/MaterialGL.h"
#include "Effects/DisplayResource/DisplayResource.h"
#include "Effects/DisplayLayer/DisplayLayer.h"
#include "GPUResources/Textures/GPUTexture2D.h"
#include "Utils/GLError.h"
#include "Utils/ImageUtilities/image_DXT.h"
//
#include "GPUResources/GPUInfo.h"


#include <cmath>

static GLenum buffers[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };


std::string toStr(int a)
{
	std::ostringstream oss;
	oss << a;
	return(oss.str());
}



GPUFBO::GPUFBO(std::string name):
	Resource(name)
{
	m_width = 0;
	m_height = 0;
	m_depth = 0;
	m_number = 0;
	depthBuffer = NULL;
	initialized = false;
	layer_bound = 0;
	oldFB = 0;
	scene = Scene::getInstance();
	HUD = new DisplayResource("HUD FBO");
	HUDArray = new DisplayLayer("HUD ARRAY FBO");
	glState = EngineGLState::getInstance();
	glCreateFramebuffers(1,&m_FBOId);
	

}


GPUFBO::~GPUFBO()
{
	//delete HUD;

	//destroy();
	//glDeleteFramebuffers(1,&m_FBOId);
}


int GPUFBO::getWidth()
{
	return m_width;
}
int GPUFBO::getHeight()
{
	return m_height;
}
bool GPUFBO::isInitialized()
{
	return initialized;
}

void GPUFBO::destroy()
{
	for (unsigned int i =0;i < colorBuffers.size();i++)
		delete colorBuffers[i];
	colorBuffers.clear();
	if (m_use_depth)
		delete depthBuffer;

}

bool GPUFBO::createTexture2DAttachments(int w,int h, int numberOfTarget,bool useDepth,GLint format)
{
	m_width = w;
	m_height = h;
	m_use_depth = useDepth;
	m_number = numberOfTarget;
	m_format = format;
	m_Target = GL_TEXTURE_2D;	

	for (int i = 0;i < m_number;i++)
	{
		createTexture2DAttachment(w,h,format);
		bindTextureToBuffer(i,i,0);
	}
	if (m_use_depth)
		createDepthAttachment(w,h);

	

	initialized = CheckFramebufferStatus();
	if (!initialized)
		LOG_ERROR << info_text << endl;
	return(initialized);


}
void GPUFBO::createTexture2DArrayAttachment(int width,int height,int layer,bool use_depth ,GLint format)
{

	m_width = width;
	m_height = height;
	m_depth = layer;
	m_use_depth = use_depth;
	m_number = 1;
	m_format = format;
	m_Target = GL_TEXTURE_2D_ARRAY;	
	
	colorBuffers.push_back(new GPUTexture2DArray(m_Name+"Color"+toStr(colorBuffers.size()),width,height,layer,format));
	if (m_use_depth)
		createDepthAttachment(width,height);

	for (int i = 0; i < layer; i++)
		bindLayerToBuffer(i, i, 0);
	initialized = CheckFramebufferStatus();
	if (!initialized)
		LOG_ERROR << info_text << endl;
	

}
bool GPUFBO::createTexture3DAttachment(int w,int h, int d,bool useDepth,GLint format )
{
	m_Target = GL_TEXTURE_3D;
	return true;

}


bool  GPUFBO::CheckFramebufferStatus()
{
	GLenum status;
	status = (GLenum) glCheckNamedFramebufferStatus(m_FBOId, GL_FRAMEBUFFER);
	bool evryok = false;
	switch(status) {
	case GL_FRAMEBUFFER_COMPLETE:
		evryok = true;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		info_text += "Unsupported framebuffer format\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		info_text += "Framebuffer incomplete, missing attachment\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		info_text += "Framebuffer incomplete, attached images must have same dimensions\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		info_text += "Framebuffer incomplete, attached images must have same format\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		info_text += "Framebuffer incomplete, missing draw buffer\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		info_text += "Framebuffer incomplete, missing read buffer\n";
		break;
	default:
		info_text += "Unknown Framebuffer Error";
	}
	return(evryok);
}

void GPUFBO::drawBuffer(int i)
{
	glNamedFramebufferDrawBuffer(m_FBOId,buffers[i]);
}
void  GPUFBO::drawBuffers(int i)
{
	glNamedFramebufferDrawBuffers(m_FBOId,i,buffers);
}

void GPUFBO::enable()
{
	
	glState->pushViewport();
	glState->setViewport(glm::ivec4(0, 0, m_width, m_height));

	glState->pushFBO();
	glState->setFBO(m_FBOId);
}

void GPUFBO::enable(int attachment, int level)
{


	bindTextureToBuffer(0,attachment,level);
		
	if (m_use_depth) 
		glNamedFramebufferTexture(m_FBOId, GL_DEPTH_ATTACHMENT, depthBuffer->getId(), level);
	enable();
}
void GPUFBO::bindTextureToBuffer(int target,int attachment, int level)
{
	glNamedFramebufferTexture(m_FBOId,buffers[target],colorBuffers[attachment]->getId(),level);		
}

void GPUFBO::bindLayerToBuffer(int target, int layer,int level)
{
	glNamedFramebufferTextureLayer(m_FBOId, buffers[target], colorBuffers[0]->getId(), level, layer);
	
}

void GPUFBO::disable()
{
	glState->popFBO();
	glState->popViewport();
}
void GPUFBO::display(const glm::vec4 &box,int layer)
{
	
	if(m_Target == GL_TEXTURE_2D)
	{
		colorBuffers[layer]->bind();
		HUD->display(box);
		colorBuffers[layer]->release();
	}
	else
	{
		colorBuffers[0]->bind();
		HUDArray->display(box,layer);
		colorBuffers[0]->release();
	}

}


void GPUFBO::displayDepth(const glm::vec4  &box)
{
	depthBuffer->bind();
	HUD->display(box);
	depthBuffer->release();

}
GPUTexture* GPUFBO::getColorTexture(int i)
{
	return colorBuffers[i];
}

GPUTexture* GPUFBO::getDepthTexture()
{
	return depthBuffer;
}

void GPUFBO::bindColorTexture(int channel,int layer )
{
	if (layer < (int)colorBuffers.size())
	{
		colorBuffers[layer]->bind(channel);
		layer_bound = layer;
	//	bufferIsBound[layer] = true;
	}
}

void GPUFBO::releaseColorTexture()
{

}

void GPUFBO::bindDepthTexture(int channel )
{
	depthBuffer->bind(channel);

}
void GPUFBO::releaseDepthTexture()
{
	depthBuffer->release();

}


GLuint GPUFBO::getFBOid()
{
	return m_FBOId;
}


void GPUFBO::createTexture2DAttachment(int width,int height,GLint format)
{
	colorBuffers.push_back(new GPUTexture2D(m_Name+"Color"+toStr(colorBuffers.size()),width,height,format));
}

void GPUFBO::createDepthAttachment(int width,int height)
{
	depthBuffer = new GPUTexture2D(m_Name + "-Depth", m_width, m_height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
	glNamedFramebufferTexture(m_FBOId, GL_DEPTH_ATTACHMENT, depthBuffer->getId(), 0);
}


void GPUFBO::changeResolution(glm::ivec2 newRes)
{
	destroy();
	createTexture2DAttachments(newRes.x, newRes.y, m_number, m_use_depth, m_format);
}
void GPUFBO::writeToFile(string filename)
{
	LOG_INFO << " Writing file" << filename <<std::endl;
	int height = this->getHeight();
	int width = this->getWidth();
	int tt = height * width * 4;
	unsigned char* toto = new unsigned char[tt];
	this->enable();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, height, width, GL_RGBA, GL_UNSIGNED_BYTE, toto);
	this->disable();
	save_image_as_DDS(filename.c_str(), width, height, 4, toto);

	
	FILE* ff = fopen(filename.c_str(), "ab");
	fwrite(toto, sizeof(unsigned char)*tt, 1, ff);

	LOG_INFO  << " File exported" << std::endl;


}
void GPUFBO::displayInterface()
{
	if (!ImGui::Begin(m_Name.c_str(), &show_interface))
	{
		ImGui::End();
		return;
	}
	int resolution = 0;

	if (ImGui::Button("Resolution"))
		ImGui::OpenPopup("selectResolution");
	ImGui::SameLine();
	std::string currentResolution = (std::to_string(m_width) + std::string(" X ") + std::to_string(m_height));
	//ImGui::Text(currentResolution.c_str());
	glm::ivec2 newResolution =glm::ivec2(0.0);
	if (ImGui::BeginPopup("selectResolution"))
	{
		ImGui::Text("%s",currentResolution.c_str());
		ImGui::Separator();
		if (ImGui::Selectable("64 X 64"))
			newResolution = glm::ivec2(64);
		if (ImGui::Selectable("128 X 128"))
			newResolution = glm::ivec2(128);
		if (ImGui::Selectable("256 X 256"))
			newResolution = glm::ivec2(256);
		if (ImGui::Selectable("512 X 512"))
			newResolution = glm::ivec2(512);
		if (ImGui::Selectable("1024 X 1024"))
			newResolution = glm::ivec2(1024);
		if (ImGui::Selectable("2048 X 2048"))
			newResolution = glm::ivec2(2048, 2048);
		if (ImGui::Selectable("4096 X 4096"))
			newResolution = glm::ivec2(4096, 4096);
		if (ImGui::Selectable("8192 X 8192"))
			newResolution = glm::ivec2(8192, 8192);
		ImGui::EndPopup();
	}
	static char buf[128] = "Screenshot.dds";
	bool to_save = false;
	
	to_save = ImGui::Button("Save");
	ImGui::SameLine();
	ImGui::Text("%s",buf);
	if (ImGui::BeginPopupContextItem("Select Filename"))
	{
		ImGui::InputText("", buf, 128);
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
	
	if (to_save)
		writeToFile(std::string(buf));
	
	if (newResolution.x != 0)
		changeResolution(newResolution);
	for (int i = 0; i < m_number; i++)
	{
		int max_width = min(128,m_width);
		int max_height = min(128,m_height);
		ImGui::Image((GLuint*)getColorTexture(i)->getId(), ImVec2(max_width, max_height), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			max_width = min(512,m_width);
			max_height = min(512,m_height);

			ImGui::Image((GLuint*)getColorTexture(i)->getId(), ImVec2(max_width, max_height), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
			ImGui::EndTooltip();
		}
		ImGui::SameLine();

	}

	
	ImGui::End();
}