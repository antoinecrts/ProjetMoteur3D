#include "XMLLoader.h"
#include "Node.h"
#include "Engine/OpenGL/SceneLoaderGL.h"
//#include "SceneLoaderGL.h"

using namespace tinyxml2;

struct mesh{
	const char * filename;
	glm::mat4 mat;	
	glm::vec3 translate;
	bool t,m,s,r;
	glm::vec3 scale;
	glm::vec3 axis;
	float angle;
	bool castShadow;
};

void readTransformation(XMLNode*n, mesh &monObj)
{
	for(XMLNode * current=n->FirstChild();current!=NULL;current=current->NextSibling())
		{
			if(strcmp(current->Value(),"scale")==0)
			{
				if(strcmp(current->ToElement()->FirstAttribute()->Name(),"value")==0)
				{//scale unique
					monObj.scale =glm::vec3(atof(current->ToElement()->FirstAttribute()->Value()));	
				}
				else
				{//scale sur les 3 axe!=
					float x = atof(current->ToElement()->FirstAttribute()->Value());
					float y = atof(current->ToElement()->FirstAttribute()->Next()->Value());
					float z = atof(current->ToElement()->FirstAttribute()->Next()->Next()->Value());
					monObj.scale = glm::vec3(x,y,z);
					monObj.s = true;
				}
			}
			if(strcmp(current->Value(),"translate")==0)
			{
				float x = atof(current->ToElement()->FirstAttribute()->Value());
				float y = atof(current->ToElement()->FirstAttribute()->Next()->Value());
				float z = atof(current->ToElement()->FirstAttribute()->Next()->Next()->Value());
				monObj.translate = glm::vec3(x,y,z);
				monObj.t = true;
			}
			if(strcmp(current->Value(),"rotate")==0)
			{
				float x = atof(current->ToElement()->FirstAttribute()->Value());
				float y = atof(current->ToElement()->FirstAttribute()->Next()->Value());
				float z = atof(current->ToElement()->FirstAttribute()->Next()->Next()->Value());
				float a = atof(current->ToElement()->FirstAttribute()->Next()->Next()->Next()->Value());
				monObj.axis = glm::vec3(x,y,z);
				monObj.angle = a;
				monObj.r = true;
			}
			if(strcmp(current->Value(),"matrix")==0)
			{
				const char* res = current->ToElement()->FirstAttribute()->Value();
				char* tmp = (char *)res;
				char * pch;
				pch = strtok(tmp," ");
				int i =0;
				monObj.mat = glm::mat4(1);
				monObj.mat[0][0] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[1][0] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[2][0] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[3][0] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[0][1] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[1][1] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[2][1] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[3][1] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[0][2] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[1][2] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[2][2] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[3][2] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[0][3] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[1][3] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[2][3] = atof(pch);pch = strtok (NULL, " ");				
				monObj.mat[3][3] = atof(pch);				
				monObj.m = true;
			}
		}	
}

mesh readShape(XMLNode *n)
{
	mesh monObj;
	monObj.t = false;
	monObj.s = false;
	monObj.m = false;
	monObj.r = false;
	monObj.castShadow = false;
	//extration du chemin
	const char* path = n->FirstChildElement()->FirstAttribute()->Next()->Value();
	// est ce un shadow caster?
	if(n->FirstChildElement()->FirstAttribute()->Next()->Next()!=0)
		if(strcmp(n->FirstChildElement()->FirstAttribute()->Next()->Next()->Value(),"True")==0)
			monObj.castShadow = true;
	monObj.filename = path;
	for(XMLNode * current=n->FirstChild();current!=NULL;current=current->NextSibling())
	{
		if(strcmp(current->Value(),"transform")==0)
			readTransformation(current,monObj);
	}
	return monObj;
}


XMLLoader::XMLLoader(string filename,string path)
{
    Scene *scene = Scene::getInstance();
 	SceneLoaderGL sceneloader;
    tinyxml2::XMLDocument doc;
    doc.LoadFile((char*)filename.c_str());
    //récupération du premier node shape
 	XMLNode * son = doc.FirstChildElement("scene")->FirstChildElement("shape");
 	/////////////////////////////////////////////////////////////////////////////
 	//          parcours des  obj                                              //
 	///////////////////////////////////////////////////////////////////////////// 
	for (XMLNode * current = son;current != NULL;current = current->NextSibling())
	{
		if (strcmp(current->ToElement()->Name(), "shape") == 0)
		{
			if (strcmp(current->ToElement()->FirstAttribute()->Value(), "obj") == 0)
			{
				mesh tmp = readShape(current);
				Node* n = sceneloader.loadScene(path + tmp.filename);
				n->displayLeaves(2);
				Node* m = n->getSon("defaultobject");
				if (m != NULL)
				{

					std::string newName(tmp.filename);
					Node* bon = scene->getNode(tmp.filename);
					bon->setModel(m->getModel());
					bon->frame()->translate(tmp.translate);
					bon->setMaterial(m->getMaterial());
					if (tmp.r) n->frame()->rotate(tmp.axis, tmp.angle);
					if (tmp.s) n->frame()->scale(tmp.scale);
					if (tmp.castShadow)
					{
						bon->getShadowModel()->setCastShadow(true);
						bon->getModel()->getGeometricModel()->buildSSBO();
					}
					bon->setName(tmp.filename);
					n->disown(m);
					scene->getRoot()->adopt(bon);
				}
				//scene->getRoot()->adopt(n);
				//delete n;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	//           recherche de la lumière                                         //
	///////////////////////////////////////////////////////////////////////////////
	XMLNode*light = doc.FirstChildElement("scene")->FirstChildElement("light");
	LightNode *t = new LightNode("lumiere", glm::vec4(1, 1, 1, 1));
	t->setModel(scene->getModel<ModelGL>(ressourceObjPath + "Sphere.obj"));
	Node* s = scene->getNode("support");
	scene->getRoot()->adopt(s);
	scene->getRoot()->adopt(t);
	if (light != 0)
	{
		float x = atof(light->ToElement()->FirstAttribute()->Value());
		float y = atof(light->ToElement()->FirstAttribute()->Next()->Value());
		float z = atof(light->ToElement()->FirstAttribute()->Next()->Next()->Value());
		t->frame()->translate(glm::vec3(x, y, z));
	}
	else //add default light
	{
		t->frame()->translate(glm::vec3(1.0));
	}

}

XMLLoader::~XMLLoader()
{

}
