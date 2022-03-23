#include "Engine/Base/Resource.h"

Resource::Resource(std::string name)
{
	m_Name = name;
	show_interface = false;
}

Resource::~Resource()
{
}

bool Resource::load()
{

	return true;
}

std::string Resource::getName()
{
	return m_Name;
}

void Resource::display(const glm::vec4 & box)
{

}

void Resource::displayInterface()
{

}