/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet
 *  Documentation : J. Lemerle P. Le Gac
 */

#include "Engine/OpenGL/v4/GLProgram.h"
#include "Engine/OpenGL/Managers/textfile.h"


GLProgramsSourceManager GLProgram::prgMgr = GLProgramsSourceManager();

GLProgram::GLProgram(std::string name,GLenum type):
m_Name(name), m_Type(type), src(NULL), m_UniformMgr(NULL)
{
	info_text = "";
	src = prgMgr.get(name,type); //permet de r�cup�rer le GLProgram correspondant au nom et au type donn�s
	if (src) m_UniformMgr = new GLUniformManager(this); //cr�ation d'un nouveau GLUniformManager afin de pouvoir utiliser des variables uniforms
}

GLProgram::~GLProgram()
{
	if (src != NULL) prgMgr.release(m_Name,m_Type); //lib�re le GLProgram
	if (m_UniformMgr != NULL) delete m_UniformMgr;

}

GLUniformManager* GLProgram::uniforms()
{
	return m_UniformMgr;
}

bool GLProgram::isValid()
{
	return(src != NULL && src->isValid());
}

GLuint GLProgram::getProgram()
{
	if (src != NULL)
		return(src->getProgram());
	else
		return -1;
}

string GLProgram::getName()
{
	string name = m_Name;
	switch (m_Type)
	{
	case GL_VERTEX_SHADER :
		name += "-VS";
		break;
	case GL_FRAGMENT_SHADER :
		name += "-FS";
		break;
	case GL_GEOMETRY_SHADER :
		name += "-GS";
		break;
	case GL_TESS_CONTROL_SHADER :
		name += "-TS";
		break;
	case GL_TESS_EVALUATION_SHADER :
		name += "-ES";
		break;
	case GL_COMPUTE_SHADER :
		name += "-CS";
		break;
	default:
		name += "Unknown";
	}

	return name;
}

GLenum GLProgram::getType()
{
	return m_Type;
}

