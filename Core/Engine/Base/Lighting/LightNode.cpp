#include "LightNode.hpp"
#include <sstream>
#include "GPUResources/FBO/GPUFBO.h"
#include "Engine/Base/Scene.h"

void printLight(Light* L)
{
}


LightNode::LightNode(std::string name,const glm::vec4 & color):
	Node(name)
{
	setColor(color);
	params.info.x = POINT_LIGHT;// par defaut on met un point light
	shadowMapSize = 512;//default shadow map res
	needUpdate = true;
	LOG_TRACE << "creating lightNode : " << name << std::endl;
}

Light LightNode::getParams()
{
	return params;
}
void LightNode::setParams(const Light & p)
{
	params = p;
}
	
glm::vec3 LightNode::getPosition()
{
	return m_Frame->convertPtTo(glm::vec3(0.0,0.0,0.0),Scene::getInstance()->getRoot()->frame());
	//return glm::vec3(params.position.x,params.position.y,params.position.z);
}
void LightNode::setPosition(const glm::vec4  p)
{
	params.position = p;
}
glm::vec4 LightNode::getColor()
{
	return params.color;
}
void LightNode::setColor(const glm::vec4  c)
{
	params.color = c;
}

float LightNode::getType()
{
	return params.info.x;
}

void LightNode::setType(unsigned int kind)
{
	params.info.x = (float)kind;
}

float LightNode::getAngle()
{
	return params.info.z;
}

void LightNode::setAngle(float a)
{
	params.info.z = a;
}


glm::vec4 LightNode::getDirection()
{
	return params.direction;
}

void LightNode::setDirection(const glm::vec4 & dir)
{
	params.direction = dir;
}


void LightNode::displayInterface()
{
	ImGui::BeginChild(getName().c_str(), ImVec2(100,100), true);
	ImGui::Text("%s",getName().c_str());
	if (ImGui::Checkbox("Manipulate Node", &isManipulated))
	{
		if (isManipulated)
			Scene::getInstance()->manipulateNode(m_Name.c_str());
		else
			Scene::getInstance()->manipulateNode("Scene");
	}
	ImGui::Text("Color :");
	ImGui::SameLine();
	glm::vec4 c = params.color;
	ImVec4 color = ImColor(c.x, c.y, c.z, c.w);
	if (ImGui::ColorButton(color))
		ImGui::OpenPopup("LightColor");
	if (ImGui::BeginPopup("LightColor"))
	{
		ImGui::ColorPicker4("", (float*)&color);
		ImGui::EndPopup();
		params.color = glm::vec4(color.x, color.y, color.z, color.w);
	}
	ImGui::EndChild();

	


}

/*
void LightNode::initShadowMap(unsigned int size = 512)
{
	cam = new Camera();
	mFBO = new GPUFBO(m_Name);
	mFBO->create(size,size);


	if(params.info.x == POINT_LIGHT)
	{// on a besoin de 2 camera
		cam2 = new Camera();
		params.info.w = 2;
		mFBO2 = new GPUFBO(m_Name+"2");
		mFBO2->create(size,size);
		//camera avec fov 180;
		
		//cam->link(Scene::getInstance()->getRoot()->frame());
		//cam2->link(Scene::getInstance()->getRoot()->frame());

		//cam->getFrame()->setUpFromMatrix(*m_Frame->getMatrix());
		//cam2->getFrame()->setUpFromMatrix(*m_Frame->getMatrix());
		//cam->setUpdate(true);
		//cam2->setUpdate(true);
		cam->getFrame()->setUpdate(true);
		cam2->getFrame()->setUpdate(true);
		cam->setPerspectiveProjection(90,1,0.1,1000 );
		cam2->setPerspectiveProjection(90,1,0.1,1000 );
		cam->lookAt(glm::vec3(1.0,0.0,0.0),glm::vec3(params.position.x,params.position.y,params.position.z),glm::vec3(0.0,0.0,1.0));
		cam2->lookAt(glm::vec3(-1.0,0.0,0.0),glm::vec3(params.position.x,params.position.y,params.position.z),glm::vec3(0.0,0.0,1.0));
		
	}
	else if(params.info.x == SPOT_LIGHT)
	{
		params.info.w = 1;
		cam->setPerspectiveProjection(params.info.z*0.5,1,1,1000);
		cam->lookAt(glm::vec3(params.direction.x,params.direction.y,params.direction.z),glm::vec3(params.position.x,params.position.y,params.position.z),glm::vec3(0.0,0.0,1.0));	
	}
	else 
	{
		params.info.w = 1;
		//cam->setOrthographicProjection();//float left,float right,float bottom,float top,float near,float far	
	}
}*/
/*
unsigned int LightNode::getNbrShadowmap()
{
	return (unsigned int)params.info.w;
}

void LightNode::enableFBO(unsigned int i)
{
	if(i==0) mFBO->enable();
	if(i==1) mFBO2->enable();
}

void LightNode::disableFBO(unsigned int i)
{
	if(i==0) mFBO->disable();
	if(i==1) mFBO2->disable();
}

Camera * LightNode::getCam(unsigned int i)
{
	if(i==0) return cam;
	if(i==1) return cam2;
}
*/
