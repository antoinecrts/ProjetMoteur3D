#pragma once
#include "Utils/Singleton.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stack>
class EngineGLState : public Singleton<EngineGLState>
{
	friend class Singleton<EngineGLState>;

	public:
		void setViewport(glm::ivec4 vp);
		void pushViewport();
		void popViewport();
		void reset();

		void setFBO(GLuint fbo);
		void pushFBO();
		void popFBO();
		void resetFBO();
	private:
		EngineGLState();
		//~EngineGLState();

		std::stack <glm::ivec4> m_ViewportState;
		std::stack <GLuint> m_FBOState;
		glm::ivec4 m_CurrentViewport;
		GLuint m_CurrentFBO;
		

};