#include "LightingModel.h"
#include "Engine/Base/Scene.h"

LightingModel::LightingModel(std::string name,unsigned int ShadowTypeInfo)
{
	m_Name = name;
	rootNode = NULL;
	needUpdate = true;
	collector = new LightCollector();
	ShadowType = ShadowTypeInfo;
	show_interface = false;
}

LightingModel::~LightingModel()
{

}

std::string LightingModel::getName()
{
	return m_Name;
}
void LightingModel::collect(Node *_rootNode)
{
	if (_rootNode == NULL)
		collector->collect(rootNode);
	else
		collector->collect(_rootNode);
}

void LightingModel::setCollector(NodeCollector *collector)
{
	if (collector != NULL)
		delete collector;
	this->collector = collector;
}

void LightingModel::update()
{
	for (vector< Node* >::iterator it = collector->nodes.begin(); it != collector->nodes.end(); ++it)
	{
		LightNode *lnode = dynamic_cast<LightNode*> (*it); 
		if (lnode != NULL && lnode->frame()->updateNeeded())
		{
			lnode->setPosition(glm::vec4(lnode->frame()->convertPtTo(glm::vec3(0.0),Scene::getInstance()->getRoot()->frame()),1.0));
			needUpdate = true;
		}
	}
}



std::vector<Node*> LightingModel::getNodes()
{
	return collector->nodes;
}

void LightingModel::displayInterface()
{

}