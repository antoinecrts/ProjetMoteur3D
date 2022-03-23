#pragma once
#include <iostream>
#include <GL/glew.h>
#include "GPUTexture.h"

class DisplayResource;

class GPUTexture2D : public GPUTexture
{
public:
	GPUTexture2D(std::string name,bool isResident = true);
	GPUTexture2D();
	GPUTexture2D(std::string name,int width,int height,GLint internalFormat = GL_RGBA8,GLint format = GL_RGBA8,GLenum type = GL_FLOAT,bool isResident = false);
	GPUTexture2D(std::string name, int width, int height, int channels, float* data, bool isResident = true);
	~GPUTexture2D();

	
	
	bool create(int width,int height,GLint internalformat = GL_RGBA8, GLint format = GL_RGBA8,GLenum type = GL_FLOAT, bool isResident = false);
	bool create(std::string filename);
	bool create(unsigned char *data, int width,int height, int channels); 
	bool create(float *data, int width, int height, int channels);
	
	int width();
	int height();
	void setUpSampler(GLint wrap,GLint minifyingFilter,GLint magnificationFilter);

	virtual void displayInterface();
	void displayInterface_content();

private:
	int m_Width;
	int  m_Height;
	int m_Channels;

};
