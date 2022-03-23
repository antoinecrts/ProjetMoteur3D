#include "ShadowModel.h"
#include "Node.h"


typedef struct remote_node{
	glm::vec4 plane;
	glm::uvec4 link;	
}r_node;

ShadowModel::ShadowModel(bool c, bool r,Node * n)
{
	m_cast 	= c;
	m_receive = r;
	if(c && n->getModel()!=NULL)
		{
			m_BufferMaxSize = 3*n->getModel()->getGeometricModel()->nb_faces;
			initSSBO();
		} 
		else m_BufferMaxSize = 0;
	inited = false;
	dad= n;
}

ShadowModel::~ShadowModel()		{}

bool ShadowModel::castShadow()				{ return m_cast;}

bool ShadowModel::receiveShadow() 			{ return m_receive;}

void ShadowModel::setCastShadow(bool b)
{
	m_cast = b;
	m_BufferMaxSize = dad->getModel()->getGeometricModel()->nb_faces;
	if (m_cast) initSSBO();
}

void ShadowModel::setReceiveShadow(bool b)	{ m_receive = b;  }

GPUBuffer * ShadowModel::getLsp2SSBO()		{ return m_nodes; }

GPUBuffer * ShadowModel::getRoots()			{ return m_roots; }

void ShadowModel::initSSBO()
{
	if (m_BufferMaxSize!=0 && !inited)
	{	
		m_nodes = new GPUBuffer("Nodes");
		if (m_nodes!=0)	
			LOG_TRACE<<"SHADOW MODEL:Nodes SSBO created size : "<<4*m_BufferMaxSize<< std::endl;;
		m_nodes->create( (1+m_BufferMaxSize) * 4 * sizeof(remote_node), GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_COPY);

		m_roots = new GPUBuffer("Roots");
		m_roots->create( 16*sizeof(GLuint), GL_SHADER_STORAGE_BUFFER ,GL_DYNAMIC_COPY);

		glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_roots->getBuffer() ); 
	
		 GLuint toto = 1;
		 glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,                sizeof(GLuint), &toto);
         glBufferSubData(GL_SHADER_STORAGE_BUFFER, 1*sizeof(GLuint), sizeof(GLuint), &toto);
         glBufferSubData(GL_SHADER_STORAGE_BUFFER, 2*sizeof(GLuint), sizeof(GLuint), &toto);
         glBufferSubData(GL_SHADER_STORAGE_BUFFER, 3*sizeof(GLuint), sizeof(GLuint), &toto);
         glBufferSubData(GL_SHADER_STORAGE_BUFFER, 4*sizeof(GLuint), sizeof(GLuint), &toto);
         glBufferSubData(GL_SHADER_STORAGE_BUFFER, 5*sizeof(GLuint), sizeof(GLuint), &toto);
         glBufferSubData(GL_SHADER_STORAGE_BUFFER, 6*sizeof(GLuint), sizeof(GLuint), &toto);
         glBufferSubData(GL_SHADER_STORAGE_BUFFER, 7*sizeof(GLuint), sizeof(GLuint), &toto);
       
         glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		inited = true;
		glFinish();
	}
}
