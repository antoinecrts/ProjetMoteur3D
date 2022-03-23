#include <iostream>
#include "Utils/tinyxml2/tinyxml2.h"
#include <string>
#include <vector> 
#include "glm/glm.hpp"
#include <stdlib.h>
#include "Scene.h"

#ifndef _XMLLOADER_
#define _XMLLOADER_

using namespace tinyxml2;

class XMLLoader
{
	public:
		XMLLoader(string filename, string path);
		~XMLLoader();
	private:
};
#endif
