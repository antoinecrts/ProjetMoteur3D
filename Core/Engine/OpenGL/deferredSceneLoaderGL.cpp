#include "deferredSceneLoaderGL.h"
#include "Materials/DeferredMaterial/DeferredMaterial.h"
Material* deferredSceneLoaderGL::parseMaterial(aiMaterial* mat)
{

	// on recupère les texture PBR en les prenant en compte comme des texture "classiques"
	// diffuse =>	albedo
	// shininess =>	roughness
	// ambient =>	metallic
	// normal =>	normal
	aiString path;
	GPUTexture2D* texAlbedo = NULL;
	GPUTexture2D* texRoughness = NULL;
	GPUTexture2D* texMetallic = NULL;
	GPUTexture2D* texNormal = NULL;
	DeferredMaterial* tmat = NULL;




	if (AI_SUCCESS == mat->GetTexture(aiTextureType_DIFFUSE, 0, &path))
	{
		texAlbedo = gobScene->getResource<GPUTexture2D>((m_basePath +"/"+ path.C_Str()));
	}
	if (AI_SUCCESS == mat->GetTexture(aiTextureType_SHININESS, 0, &path)) {
		texRoughness = gobScene->getResource<GPUTexture2D>((m_basePath + "/" + path.C_Str()));
	}
	if (AI_SUCCESS == mat->GetTexture(aiTextureType_AMBIENT, 0, &path)) {
		texMetallic = gobScene->getResource<GPUTexture2D>((m_basePath + "/" + path.C_Str()));
	}
	if (AI_SUCCESS == mat->GetTexture(aiTextureType_HEIGHT, 0, &path)) {
		texNormal = gobScene->getResource<GPUTexture2D>((m_basePath + "/" + path.C_Str()));
	}
	//	
	
	tmat = new DeferredMaterial("DeferredMaterial",
	texAlbedo,
	NULL,
	texMetallic,
	texRoughness,
	NULL,
	texNormal,
	0.05f, 32, false, false,
	glm::vec3(0.5),
	glm::vec3(0.5));
	

	return tmat;
}