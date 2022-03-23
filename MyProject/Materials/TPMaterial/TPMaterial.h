#ifndef _TPMATERIAL_H
#define _TPMATERIAL_H


#include "Engine/OpenGL/MaterialGL.h"
#include "Engine/OpenGL/Lighting/LightingModelGL.h"
#include <memory.h>
#include <string>


class TPMaterial : public MaterialGL
{
	public:
		TPMaterial(std::string name);
		~TPMaterial();
		void setColor(glm::vec4 &c);

		virtual void render(Node *o);
		virtual void update(Node* o,const int elapsedTime);
		GPUmat4* modelViewProj;
		GPUmat4* ModelM;
		GPUvec3* PartPos;
		GPUvec3* CamPos;
		
		/* GPUTexture2D la classe pour charger les textures */
		GPUTexture2D* text;
		GPUTexture2D* text_n;
		GPUTexture2D* text2;
		GPUTexture2D* text3;
		GPUTexture2D* text4;

		/* On utilise des GPUsampler pour donner la texture au fragment shader */
		GPUsampler* difftext;
		GPUsampler* normtext;
		GPUsampler* heitext;
		GPUsampler* aotext;
		GPUsampler* roughtext;
};

#endif