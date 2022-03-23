
#ifndef _LIGHTING_MODEL_
#define _LIGHTING_MODEL_
#include "LightNode.hpp"
#include "Engine/Base/NodeCollectors/LightCollector.h"
#include "Utils/Logger/ImGuiLogger.h"

#define NONE 			0
#define SHADOW_MAP 		1
#define SHADOW_VOLUME 	2
#define SHADOW_PLUCKER 	3




class LightingModel 
{

public :
	LightingModel(std::string name = "",unsigned int ShadowTypeInfo = NONE);
	~LightingModel();

	std::string getName();

	void collect(Node *_rootNode = NULL);
	void setCollector(NodeCollector *collector);
	std::vector<Node*> getNodes();

	virtual void update();
	bool show_interface;
	virtual void displayInterface();
protected:
	std::string m_Name;	
	NodeCollector* collector;
	bool needUpdate;
	unsigned int ShadowType;
	Node * rootNode;
};

#endif
	
