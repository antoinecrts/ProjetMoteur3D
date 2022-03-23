/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet
 *  Documentation : J. Lemerle P. Le Gac
 */

#include "Engine/OpenGL/MaterialGL.h"
#include "Engine/Base/Node.h"

#include "CameraGL.h"
#include "Engine/OpenGL/Lighting/LightingModelGL.h"

MaterialGL::MaterialGL(string name, string className) :
	Material(name, className), m_ProgramPipeline(NULL), vp(NULL), tcp(NULL), tep(NULL), gp(NULL), fp(NULL)
{
	try
	{
		GLProgram::prgMgr.addPath(ressourceMaterialPath + this->m_ClassName,this->m_ClassName);
		GLProgram::prgMgr.addPath(ressourceCoreMaterialPath + this->m_ClassName, this->m_ClassName);
		
		m_ProgramPipeline = new GLProgramPipeline(this->m_ClassName);
		// autoloading existing programm
		vp = new GLProgram(this->m_ClassName + "-Main", GL_VERTEX_SHADER); // Création instance GLProgram pour vertex shader
		tcp = new GLProgram(this->m_ClassName + "-Main", GL_TESS_CONTROL_SHADER); // instance pour tesselation control shader
		tep = new GLProgram(this->m_ClassName + "-Main", GL_TESS_EVALUATION_SHADER); // instance pour tesselation evaluation shader
		gp = new GLProgram(this->m_ClassName + "-Main", GL_GEOMETRY_SHADER); // instance pour geometry shader
		fp = new GLProgram(this->m_ClassName + "-Main", GL_FRAGMENT_SHADER); // instance pour fragment shader

		// Load each valid stage to the pipeline
		if (vp != NULL && vp->isValid()) 	m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT, vp);
		if (tcp != NULL && tcp->isValid()) 	m_ProgramPipeline->useProgramStage(GL_TESS_CONTROL_SHADER_BIT, tcp);
		if (tep != NULL && tep->isValid()) 	m_ProgramPipeline->useProgramStage(GL_TESS_EVALUATION_SHADER_BIT, tep);
		if (gp != NULL && gp->isValid()) 	m_ProgramPipeline->useProgramStage(GL_GEOMETRY_SHADER_BIT, gp);
		if (fp != NULL && fp->isValid()) 	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, fp);

		valid = m_ProgramPipeline->link();

		
		
	}
	catch(const std::exception & e )
	{
		
		//if (m_ProgramPipeline != NULL) 
		//	delete m_ProgramPipeline;
		
		//suppression des shaders si ils existent
		if (vp != NULL) 
			delete vp;
		if (fp != NULL)	
			delete fp;
		if (gp != NULL)	
			delete gp;
		if (tcp != NULL) 
			delete tcp;
		if (tep != NULL) 
			delete tep;
		
		//On modifie le matériau prévu par un matériau basique
		GLProgram::prgMgr.addPath(ressourceCoreMaterialPath + "BaseMaterial", "BaseMaterial");
		
		vp = new GLProgram("BaseMaterial-Main", GL_VERTEX_SHADER);
		fp = new GLProgram("BaseMaterial-Main", GL_FRAGMENT_SHADER);
		m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT, vp);
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, fp);
		valid = m_ProgramPipeline->link();
		m_Name = " ERROR : " + name;
		LOG_ERROR << std::string("Material ") << this->m_ClassName << string(" : \n") << e.what()  << std::endl;
		Logger::getInstance()->show_interface = true;
	}

	
	// Binding manuel des blocks sur un point de binding : au cas ou le binding= ne marcherait pas
	// Peut être le cas sur les cartes AMD (mais à re tester avec les derniers drivers)
	
	GLuint block_index = 0;
	block_index = glGetProgramResourceIndex(vp->getProgram(), GL_SHADER_STORAGE_BLOCK, "matricesBuffer");
	if (block_index != GL_INVALID_INDEX)
	{
		glShaderStorageBlockBinding(vp->getProgram(), block_index, COMMON_SSBO_BINDING);
	}
	
	block_index = glGetProgramResourceIndex(fp->getProgram(), GL_SHADER_STORAGE_BLOCK, "LightingBuffer");
	if (block_index != GL_INVALID_INDEX)
	{
		glShaderStorageBlockBinding(fp->getProgram(), block_index, LIGHTING_SSBO_BINDING);
	}
	
}

MaterialGL::~MaterialGL()
{
	if (vp != NULL)
		delete vp;
	if (fp != NULL)
		delete fp;
	if (gp != NULL)
		delete gp;
	if (tcp != NULL)
		delete tcp;
	if (tep != NULL)
		delete tep;
	if (m_ProgramPipeline != NULL)
		delete m_ProgramPipeline;
}

void MaterialGL::render(Node *o)
{

}
