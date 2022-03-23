#pragma once
#include <string>
#include <stdlib.h>
#include <glm/glm.hpp>
#include "Utils/Logger/ImGuiLogger.h"
#include <algorithm>
class Resource
{
public:
	Resource(std::string name);
	~Resource();

	std::string getName();
	virtual void display(const glm::vec4 & box = glm::vec4(0.0,0.0,1.0,1.0));
	virtual void displayInterface();
	bool show_interface;
protected:
	std::string m_Name;
	virtual bool load();

};
	  