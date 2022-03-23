/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet
 *
 */
#include "Engine/Base/Node.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "Engine/Base/Scene.h"
#include "Engine/Base/BoundingBox/BoundingBox.h"

class Material;

Node::Node(std::string name)
{
	m_Name = name;
	m_Material = NULL;
	m_Model = NULL;
	m_Animator = NULL;
	m_Shadow = NULL;
	m_Model = NULL;
	isManipulated = false;

	// Frame Creation
	m_Frame = new Frame();

	box = new BoundingBox(this);
	show_interface = false;


}

Node::Node(const Node& toCopy)
{
	// Copy everything
	this->m_Animator = (toCopy.m_Animator);
	this->m_Father = (toCopy.m_Father);
	this->m_Frame = (toCopy.m_Frame);
	this->m_Material = (toCopy.m_Material);
	this->m_Model = (toCopy.m_Model);

	this->m_Name = string(toCopy.m_Name + "-copy" );
	this->m_Shadow = toCopy.m_Shadow;
	this->m_Sons = toCopy.m_Sons;
}

Node::~Node()
{
	LOG_TRACE << "Deleting Node : " << m_Name << std::endl;

	delete m_Frame;
	delete m_Animator;
	delete m_Shadow;
}

const std::string Node::getName()
{
	return(m_Name);
}

void Node::setName(std::string name)
{
	m_Name = name;
}

//void Node::computeAABB()
//{
	//if (m_Model != NULL)
	//	AABB.computeAABB(m_Model->getGeometricModel()->box,this->frame());
		
//}
void Node::setModel(Model *m)
{
	m_Model = m;
	//m_Shadow = new ShadowModel(false,this);
	m_Shadow = new ShadowModel(false, false, this);
	box->setModel(m);
}
Model* Node::getModel()
{
	return m_Model;
}
void Node::drawGeometry(int type)
{
	m_Model->drawGeometry(type);
}
void Node::setMaterial(Material *m,bool recurse)
{
	if (m_Model)
		m_Material = m;
	if (recurse)
		for(unsigned int i = 0;i < m_Sons.size();i++)
			m_Sons[i]->setMaterial(m,recurse);
}
Material* Node::getMaterial()
{
	return(m_Material);
}

void Node::render(Material* mat)
{
	
	if (m_Model)
		if (mat) 
			mat->render(this);
		else if (m_Material != NULL) 
			m_Material->render(this);
}

void Node::animate(const int elapsedTime)
{
	if(m_Animator)
	{
		m_Animator->animate(elapsedTime, frame()); // a definir
	}
	if (m_Material)
		m_Material->update(this,elapsedTime);

	//if (frame()->updateNeeded())
	//	this->box->computeWorldAABB();
}

void Node::adopt(Node* son)
{
	son->m_Father = this;
	son->frame()->attachTo(m_Frame);
	m_Sons.push_back(son);
}
Frame* Node::frame()
{
	return m_Frame;
}

void Node::setAnimator(Animator* a)
{
	m_Animator = a;
}

Animator* Node::animator()
{
	return m_Animator;
}


void Node::displayLeaves(int level)
{
	std::string toDisplay = std::string(" - " + m_Name);
	LOG_INFO<< toDisplay ;
	if (m_Sons.size() == 0)
	{
		LOG_INFO<< ";" << std::endl ;
	}
	else
	{
		int display_offset = level + toDisplay.size();
		m_Sons[0]->displayLeaves(display_offset);
		if (m_Sons.size() > 1)
		{
			for (int i = 1; i < m_Sons.size(); i++)
			{
				for (int j = 0; j < display_offset; j++)
				{
					LOG_INFO << " " ;
				}
				LOG_INFO<< "|" << endl;

				for (int j = 0; j < display_offset; j++)
				{
					LOG_INFO<< " ";
				}
				m_Sons[i]->displayLeaves(display_offset);
			}
		}
	}
}

bool Node::disown(Node* son)
{
	bool isInLeaves = false;
	for (int i = 0; i < m_Sons.size() && !isInLeaves; i++)
	{
		if (m_Sons[i] == son)
		{
			m_Frame->detach(son->frame());
			m_Sons.erase(m_Sons.begin() + i);
			return true;
		}
		else
		{
			isInLeaves = m_Sons[i]->disown(son);
		}
	}
	return isInLeaves;
}

Node* Node::getSon(std::string name)
{
	for (int i = 0;i <= m_Sons.size();i++)
		if (m_Sons[i]->getName() == name) return m_Sons[i];

	return NULL;
}

ShadowModel *Node::getShadowModel()
{
	return m_Shadow;
}

void Node::displayInterface()
{
	if (!ImGui::Begin(m_Name.c_str(), &show_interface))
	{
		ImGui::End();
		return;
	}
	if (ImGui::Checkbox("Manipulate Node", &isManipulated))
	{
		if (isManipulated)
			Scene::getInstance()->manipulateNode(m_Name.c_str());
		else
			Scene::getInstance()->manipulateNode("Scene");
	}


	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.06f, 0.86f, 0.05f, 0.45f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered,ImVec4(0.38f, 0.98f, 0.33f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.38f, 0.98f, 0.33f, 0.30f));
	if (m_Material !=NULL)
	{
		if (ImGui::CollapsingHeader(m_Material->getName().c_str()))
			m_Material->displayInterface();
	}
	ImGui::PopStyleColor(3);
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.86f, 0.8f, 0.05f, 0.55f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.86f, 0.8f, 0.15f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.86f, 0.8f, 0.15f, 0.30f));
	if (m_Model != NULL)
	{
		std::string name = m_Model->getName();
		if (name.size() == 0)
			name = "Model";
		if (ImGui::CollapsingHeader(name.c_str()))
			m_Model->displayInterface();
	}
		

	ImGui::PopStyleColor(3);
	ImGui::End();
}