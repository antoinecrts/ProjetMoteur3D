#ifndef _GBUFFERS_MATERIAL
#define _GBUFFERS_MATERIAL

#include "Engine/OpenGL/MaterialGL.h"
#include "GPUResources/Textures/GPUTexture2D.h"

#include <memory.h>

class GBuffersMaterial : public MaterialGL
{
	public:
		GBuffersMaterial(std::string name);
		~GBuffersMaterial();

		virtual void render(Node *o);
		void updateCamera();
		GPUmat4* modelViewProj;
		GPUmat4* vs_M,*vs_NormalM;

		GPUmat4* modelViewProj_g;
		GPUmat4* modelView_g;

		GPUmat4* normalModelView;
		GPUmat4* modelMatrix;
		GLProgram *cp,*ep,*gp;

		void setLightTexture(GPUTexture2D* _lm_text);
		void update(Node* o, const int elapsedTime);


	protected:
		bool loadShaders();
		GPUTexture2D* lm;
		GPUsampler *lm_sampler;
};
#endif