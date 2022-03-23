#include "Materials/DeferredMaterial/DeferredMaterial.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"


DeferredMaterial::DeferredMaterial (std::string name, 
	GPUTexture *color, 
	GPUTexture *ao, 
	GPUTexture *metalMask, 
	GPUTexture *roughness, 
	GPUTexture *reflectance, 
	GPUTexture *normal, 
	float bumpScale, 
	float maxQuality, 
	bool height, 
	bool ss, 
	glm::vec3 metal_reflectance_roughness,
	glm::vec3 baseColor): 
MaterialGL(name, "DeferredMaterial")
{

	GPUTexture *fs_colorTex, *fs_aoTex, *fs_metalMaskTex, *fs_roughnessTex, *fs_reflectanceTex, *fs_normalTex;
	GPUsampler *fs_colorSampler, *fs_aoSampler, *fs_metalMaskSampler, *fs_roughnessSampler, *fs_reflectanceSampler, *fs_normalSampler;

	fs_colorTex = NULL; fs_aoTex = NULL; fs_metalMaskTex = NULL; fs_roughnessTex = NULL; fs_reflectanceTex = NULL; fs_normalTex = NULL;
	
	//----- Vertex Program Parameters ---------------------//
	this->vs_LastMVP = vp->uniforms ()->getGPUmat4 ("lastMVP");
	this->vs_NormalM = vp->uniforms ()->getGPUmat4 ("NormalM");
	this->vs_M = vp->uniforms ()->getGPUmat4 ("M");
	//----- Tesselation Control Program Parameters --------//
	//----- Tesselation Evaluation Program Parameters -----//
	//----- Geometry Program Parameters -------------------//
	//this->gs_MV = gp->uniforms()->getGPUmat4("MV");
	//this->gs_MVP = gp->uniforms()->getGPUmat4("MVP");
	//this->gs_NormalMV = gp->uniforms()->getGPUmat4("NormalMV");
	//this->gs_CamPos = gp->uniforms()->getGPUvec3("CamPos");
	//----- Fragment Program Parameters -------------------//
	this->fs_bumpScale = fp->uniforms()->getGPUfloat("bumpScale");
	this->fs_bumpScale->Set(bumpScale);
	this->fs_maxQuality = fp->uniforms()->getGPUfloat("MaxQuality");
	this->fs_maxQuality->Set(maxQuality);
	

	fs_baseColor = fp->uniforms ()->getGPUvec3 ("base_Color");

	//----- CLASSIC TEXTURE USE ---------------------------//

	// Diffuse Mapping
	fs_colorSampler = fp->uniforms ()->getGPUsampler ("baseColorTex");
	fs_use_base_color = fp->uniforms ()->getGPUbool ("use_base_color");
	if (color != NULL) {
		fs_colorTex = color;
		fs_colorSampler->Set (fs_colorTex->getHandle ());
		fs_use_base_color->Set (true);
	}
	else {
		fs_use_base_color->Set (false);
		fs_baseColor->Set (baseColor);
	}
	// AO Mapping
	fs_aoSampler = fp->uniforms()->getGPUsampler("aoTex");
	fs_use_ao = fp->uniforms()->getGPUbool("use_ao");
	if (ao != NULL)
	{
		fs_aoTex = ao;
		fs_aoSampler->Set(fs_aoTex->getHandle());
		fs_use_ao->Set(true);
	}
	else
		fs_use_ao->Set(false);

	fs_metalMask_reflectance_roughness = fp->uniforms ()->getGPUvec3 ("metalMask_Reflectance_Roughness");
	fs_metalMask_reflectance_roughness->Set (metal_reflectance_roughness);


	// MetalMask Mapping
	fs_metalMaskSampler = fp->uniforms()->getGPUsampler("metalMaskTex");
	fs_use_metalMask = fp->uniforms()->getGPUbool("use_metal_mask");
	if (metalMask != NULL)
	{
		fs_metalMaskTex = metalMask;
		fs_metalMaskSampler->Set (fs_metalMaskTex->getHandle());
		fs_use_metalMask->Set (true);
	}
	else
		fs_use_metalMask->Set (false);

	//
	fs_roughnessSampler = fp->uniforms()->getGPUsampler("roughnessTex");
	fs_use_roughness = fp->uniforms ()->getGPUbool ("use_roughness");
	if (roughness != NULL)
	{
		fs_roughnessTex = roughness;
		fs_roughnessSampler->Set (fs_roughnessTex->getHandle());
		fs_use_roughness->Set (true);
	}
	else
		fs_use_roughness->Set (false);

	//
	fs_reflectanceSampler = fp->uniforms ()->getGPUsampler ("reflectanceTex");
	fs_use_reflectance = fp->uniforms ()->getGPUbool ("use_reflectance");
	if (reflectance != NULL)
	{
		fs_reflectanceTex = reflectance;
		fs_reflectanceSampler->Set (fs_reflectanceTex->getHandle());
		fs_use_reflectance->Set (true);
	}
	else
		fs_use_reflectance->Set (false);

	// Normal Mapping
	fs_normalSampler = fp->uniforms()->getGPUsampler("normalTex");
	fs_use_normal = fp->uniforms()->getGPUbool("use_normal");
	if (normal != NULL)
	{
		fs_normalTex = normal;
		fs_normalSampler->Set(fs_normalTex->getHandle());
		fs_use_normal->Set(true);
	}
	else
		fs_use_normal->Set(false);

	// Parallax Mapping
	fs_use_height = fp->uniforms()->getGPUbool("use_height");
	fs_use_height->Set(height);
	fs_use_selfshadow = fp->uniforms()->getGPUbool("use_selfshadow");
	fs_use_selfshadow->Set(ss && height);

}

DeferredMaterial::~DeferredMaterial(){}

void DeferredMaterial::render(Node *o)
{
	
	if (m_ProgramPipeline)
	{
		//m_ProgramPipeline->bind();
		m_ProgramPipeline->bindUniforms();
		o->drawGeometry(GL_TRIANGLES);
		//m_ProgramPipeline->release();


	}
}

void DeferredMaterial::update(Node* o, const int elapsedTime)
{

	Scene* scene = Scene::getInstance ();
	glm::mat4 MVP = o->frame()->getTransformMatrix();
	glm::mat4 M = o->frame ()->getRootMatrix ();
	glm::mat4 NormalM = glm::transpose (glm::inverse (M));
	vs_LastMVP->Set (oldMVP);
	oldMVP = MVP;

	
	if (o->frame()->updateNeeded())
	{
		vs_M->Set (M);
		vs_NormalM->Set (NormalM);
	}
	
}