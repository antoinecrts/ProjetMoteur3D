#include "GPUResources/Textures/GPUTexture2D.h"
#include "Effects/DisplayResource/DisplayResource.h"
#include <SOIL/SOIL.h>
#include "Utils/ImageUtilities/ImageUtils.h"
#include "Utils/ImageReader/ImageReader.h"

#include "Utils/GLError.h"

GPUTexture2D::GPUTexture2D(std::string name,bool isResident):
GPUTexture(name)
{
	m_Target = GL_TEXTURE_2D;
	create(name);
	resident = isResident;
	m_Channels = 0;
	if (resident)
		makeResident();

}
GPUTexture2D::GPUTexture2D():
GPUTexture("")
{
	m_Target = GL_TEXTURE_2D;
	m_Channels = 0;

}
GPUTexture2D::GPUTexture2D(std::string name,int width,int height,GLint internalformat,GLint format ,GLenum type, bool isResident):
GPUTexture(name)
{
	resident = isResident;
	m_Target = GL_TEXTURE_2D;
	
	create(width,height,internalformat,format,type);

	if (resident)
		makeResident();
}
GPUTexture2D::GPUTexture2D(std::string name, int width, int height, int channels, float* data, bool isResident) :
GPUTexture(name)
{
	resident = isResident;
	m_Target = GL_TEXTURE_2D;

	create(data, width, height, channels);

	if (resident)
		makeResident();
}
void GPUTexture2D::setUpSampler(GLint wrap,GLint minifyingFilter,GLint magnificationFilter)
{
	glBindSampler(m_TextureId,m_SamplerId);
	glSamplerParameteri(m_SamplerId , GL_TEXTURE_WRAP_S, wrap);
	glSamplerParameteri(m_SamplerId , GL_TEXTURE_WRAP_T, wrap);
    glSamplerParameteri(m_SamplerId , GL_TEXTURE_MIN_FILTER , minifyingFilter);
    glSamplerParameteri(m_SamplerId , GL_TEXTURE_MAG_FILTER , magnificationFilter);
}


bool GPUTexture2D::create(int width,int height,GLint internalformat, GLint format,GLenum type, bool isResident)
{
	resident = isResident;
	m_format = format;
	glGenTextures(1,&m_TextureId);
	glBindTexture(m_Target,m_TextureId);
	glTexStorage2D(m_Target,floor(log2(min(height,width))),internalformat ,width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	setUpSampler(GL_REPEAT,GL_LINEAR,GL_LINEAR);
	m_Width = width;
	m_Height = height;

	if (resident)
		makeResident();
	
	return (true);
}



bool GPUTexture2D::create(std::string filename)
{
	m_TextureId = 0;
	//m_TextureId = load2DTexture(filename, SOIL_LOAD_AUTO);
	int width,height,channels;

	
	unsigned char * tex = NULL;
	tex = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
	if (tex == NULL)
		LOG_WARNING << "texture " << filename << " not found " << std::endl;



	glGenTextures(1,&m_TextureId);
	glBindTexture(m_Target,m_TextureId);
	int ttype = GL_RGBA8;
	if (channels == 3)
		ttype = GL_RGB8;
	if (channels == 2)
		ttype = GL_RG8;
	if (channels == 1)
		ttype = GL_R8;
	glTexStorage2D(m_Target, floor(log2(min(height,width))), ttype, width, height);
			
	ttype = GL_RGBA;
	if (channels == 3)
	ttype = GL_RGB;
	if (channels == 2)
		ttype = GL_RG;
	if (channels == 1)
		ttype = GL_RED;
	glTexSubImage2D(m_Target, 0, 0, 0, width, height, ttype, GL_UNSIGNED_BYTE, tex);

	m_Width = width;
	m_Height = height;
	m_Channels = channels;
	
	/*
	m_TextureId = SOIL_load_OGL_texture
	(
		filename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	*/
	/*
GFLImageReader *r = new GFLImageReader();

	m_TextureId = r->load2DTexture(filename);
*/






	
	glGenerateMipmap(m_Target);

	if (m_TextureId == 0)
	{
		LOG_ERROR << " GPUTexture2D - SOIL loading error in file " << filename << " : " << SOIL_last_result() << std::endl;
		return false;
	}

	setUpSampler(GL_REPEAT,GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);

	return (m_TextureId != 0);
}

bool GPUTexture2D::create(unsigned char *data, int width,int height, int channels)  
{
	
	glGenTextures(1, &m_TextureId);
	glBindTexture(m_Target, m_TextureId);

	int ttype = GL_RGBA8;
	if (channels == 3)
		ttype = GL_RGB8;
	if (channels == 2)
		ttype = GL_RG8;
	if (channels == 1)
		ttype = GL_R8;
	glTexStorage2D(m_Target, floor(log2(min(height,width))), ttype, width, height);

	ttype = GL_RGBA;
	if (channels == 3)
		ttype = GL_RGB;
	if (channels == 2)
		ttype = GL_RG;
	if (channels == 1)
		ttype = GL_RED;

	glTexSubImage2D(m_Target, 0, 0, 0, width, height, ttype, GL_UNSIGNED_BYTE, data);

	m_Width = width;
	m_Height = height;
	if (resident)
		makeResident();
	return (m_TextureId != 0);
}
bool GPUTexture2D::create(float *data, int width, int height, int channels)
{

	glGenTextures(1, &m_TextureId);
	glBindTexture(m_Target, m_TextureId);

	int ttype = GL_RGBA8;
	if (channels == 3)
		ttype = GL_RGB8;
	if (channels == 2)
		ttype = GL_RG8;
	if (channels == 1)
		ttype = GL_R8;
	glTexStorage2D(m_Target, floor(log2(min(height,width))), ttype, width, height);

	ttype = GL_RGBA;
	if (channels == 3)
		ttype = GL_RGB;
	if (channels == 2)
		ttype = GL_RG;
	if (channels == 1)
		ttype = GL_RED;

	glTexSubImage2D(m_Target, 0, 0, 0, width, height, ttype, GL_FLOAT, data);

	m_Width = width;
	m_Height = height;
	if (resident)
		makeResident();
	return (m_TextureId != 0);

}
GPUTexture2D::~GPUTexture2D()
{
	
}
void GPUTexture2D::displayInterface()
{
	
	if (!ImGui::Begin(m_Name.c_str(), &show_interface))
	{
		ImGui::End();
		return;
	}
	displayInterface_content();
	ImGui::End();
	
}
void GPUTexture2D::displayInterface_content()
{
	
	 ImGui::Text("Texture Size: %.1i x %.1i",m_Width,m_Height);
	 ImGui::Text("Channels: %.1i ",m_Channels);
	 ImGui::Image((GLuint*)getId(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
	
	
}