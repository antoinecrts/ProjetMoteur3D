#ifndef _CONSTANTS_H
#define _CONSTANTS_H
#include <string>

const std::string rootPath = "./MyProject/";
const std::string corePath = "./Core/";


#define USE_ASSIMP
const std::string ressourceMaterialPath = rootPath+"Materials/";
const std::string ressourceCoreMaterialPath = corePath +"Materials/";

const std::string ressourceEffectPath = rootPath+"Effects/";
const std::string ressourceCoreEffectPath =  corePath +"Effects/";

const std::string ressourceObjPath = rootPath+"Objets/";
const std::string ressourceCoreObjPath = corePath + "Objets/";

const std::string ressourceGPUPath = rootPath+"Resources/";
const std::string ressourceTexPath = rootPath+"Textures/";

#define LIGHTING_SSBO_BINDING 20
#define COMMON_SSBO_BINDING 21


#define HD 1

#if HD
#define FBO_WIDTH 1920
#define FBO_HEIGHT 1080
#else 
#define FBO_WIDTH 1280
#define FBO_HEIGHT 720
#endif


#endif
