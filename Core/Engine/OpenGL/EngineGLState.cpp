#include "EngineGLState.h"

EngineGLState::EngineGLState()
{
	m_CurrentFBO = 0;

}
void EngineGLState::setViewport(glm::ivec4 vp)
{
	m_CurrentViewport = vp;
	glViewport(m_CurrentViewport.x, m_CurrentViewport.y, m_CurrentViewport.z, m_CurrentViewport.w);
}
void EngineGLState::pushViewport()
{
	m_ViewportState.push(m_CurrentViewport);
}
void EngineGLState::popViewport()
{
	m_CurrentViewport = m_ViewportState.top();
	m_ViewportState.pop();
	glViewport(m_CurrentViewport.x, m_CurrentViewport.y, m_CurrentViewport.z, m_CurrentViewport.w);
}
void EngineGLState::reset()
{
	while (!m_ViewportState.empty())
	{
		m_ViewportState.pop();
	}
}

void EngineGLState::setFBO(GLuint fbo)
{
	m_CurrentFBO = fbo;
	glBindFramebuffer (GL_FRAMEBUFFER, fbo);
}

void EngineGLState::pushFBO()
{
	m_FBOState.push(m_CurrentFBO);
}
void EngineGLState::popFBO()
{
	m_CurrentFBO =m_FBOState.top();
	m_FBOState.pop();
	glBindFramebuffer (GL_FRAMEBUFFER, m_CurrentFBO);
}
void EngineGLState::resetFBO()
{
	while (!m_ViewportState.empty())
	{
		m_FBOState.pop();
	}
	m_CurrentFBO = 0;

	glBindFramebuffer (GL_FRAMEBUFFER, m_CurrentFBO);
}