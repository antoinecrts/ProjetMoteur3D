#ifndef _EFFECTGL_
#define _EFFECTGL_

#include "Engine/Base/Effect.h"
#include "Engine/OpenGL/v4/GLProgramPipeline.h"
#include "GPUResources/FBO/GPUFBO.h"
#include "Engine/OpenGL/ModelGL.h"
#include "Utils/imgui/imgui.h"
#include "Utils/imgui/imgui_impl_glfw_gl3.h"


class EffectGL : public Effect
{
	public :
		EffectGL(string name,string className);
		~EffectGL();

		//virtual void apply();
		virtual void apply(GPUFBO *fbo=NULL);

		virtual void displayInterface();

	protected :
		void drawQuad();
		
		GLProgramPipeline* m_ProgramPipeline;
		ModelGL *quad;
};

#endif
