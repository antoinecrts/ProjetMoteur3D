#include "Engine/Base/Scene.h"
#include "Lighting/LightNode.hpp"

Scene::Scene()
{

	LOG_TRACE << "Creating Scene" << std::endl;
	// Get the root Node 
	m_Root = m_Nodes.get("Root");
	m_Frame = m_Root->frame();
	// Create a projective default camera with standard parameter and place it in the scene
	CameraGL* camera = m_Cameras.get("DefaultCamera");
	camera->setPerspectiveProjection(45.0f, 1.77777f, 0.01f, 2000.0f);

	camera->lookAt(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,10.0,10.0),glm::vec3(0.0,1.0,0.0));
	
	current_Camera = camera;
		
	// Link the camera frame to the scene frame
	camera->link(m_Frame);	

	// Create a Scene Node and link it to root node
	m_Scene = m_Nodes.get("Scene");
	m_Root->adopt(m_Scene);
	current_ManipulatedNode = m_Scene;
}

Scene::~Scene()
{
	//m_Nodes.release("Root");
}
Frame* Scene::frame()
{
	return m_Frame;
}
CameraGL* Scene::camera()
{
	return current_Camera;
}

void Scene::nextCamera()
{
	current_Camera = m_Cameras.nextObject(current_Camera->getName());
	current_Camera->link(m_Frame);
	current_Camera->bind();

	LOG_INFO << "using camera " << current_Camera->getName() << std::endl;
}
void Scene::nextManipulatedNode()
{
	current_ManipulatedNode->isManipulated = false;
	current_ManipulatedNode = m_Nodes.nextObject(current_ManipulatedNode->getName());
	if (current_ManipulatedNode == m_Root)
		current_ManipulatedNode = m_Nodes.nextObject(current_ManipulatedNode->getName());
	current_ManipulatedNode->isManipulated = true;
	LOG_INFO << "manipulating Node " << current_ManipulatedNode->getName() << std::endl;
}
void Scene::manipulateNode(std::string name)
{
	Node* c_node =  m_Nodes.find(name);
	if (c_node == NULL)
		LOG_WARNING << "Error : Node " << name << " does not exists." << std::endl;
	else
	{
		if (current_ManipulatedNode != NULL)
			current_ManipulatedNode->isManipulated = false;
		current_ManipulatedNode = c_node;
		c_node->isManipulated = true;
		LOG_INFO << "Manipulated node is now " << current_ManipulatedNode->getName() << std::endl;
	}
}

Node* Scene::getSceneNode()
{
	return m_Scene;
}
Node* Scene::getManipulatedNode()
{
	return current_ManipulatedNode;
}

void Scene::useCamera(std::string name)
{
	
	CameraGL* cam = m_Cameras.find(name);
	if (cam == NULL)
		LOG_WARNING << "Error : Camera " << name << " does not exists " << std::endl;
	else
	{
		current_Camera = cam;
		cam->link(m_Frame);
		cam->bind();
		LOG_INFO << "Rendering to camera " << cam->getName() << std::endl;
	}
}
CameraGL* Scene::getCamera(std::string name)
{
	return (m_Cameras.find(name));
}


Node* Scene::getRoot()
{
	return m_Root;
}
Node* Scene::getNode(std::string name)
{
	return(m_Nodes.get(name));
}
void Scene::releaseNode(Node *n)
{
	m_Nodes.release(n->getName());
}
void Scene::releaseNode(std::string name)
{
	m_Nodes.release(name);
}

void Scene::releaseResource(string a)
 {
	m_Resources.release(a);
 }
void Scene::releaseResource(Resource *r)
{
	m_Resources.release(r->getName());
}

void Scene::releaseModel(string a)
{
	m_Models.release(a);
}
void Scene::releaseModel(Model *m)
{
	m_Models.release(m->getName());
}

void Scene::releaseEffect(string a)
{
	m_Effects.release(a);
}
void Scene::releaseEffect(Effect *m)
{
	m_Effects.release(m->getName());
}
void Scene::displayInterface()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Scene Information"))
		{
			if (ImGui::BeginMenu("Nodes"))
			{
				for (unsigned int i = 0; i < m_Nodes.size(); i++)
					if (dynamic_cast<LightNode*> (m_Nodes.get(i)) == NULL)
						ImGui::MenuItem(m_Nodes.get(i)->getName().c_str(), NULL, &(m_Nodes.get(i)->show_interface));
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Resources"))
			{
				for (unsigned int i = 0; i <m_Resources.size(); i++)
					ImGui::MenuItem(m_Resources.get(i)->getName().c_str(), NULL, &(m_Resources.get(i)->show_interface));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Effects"))
			{
				for (unsigned int i = 0; i <m_Effects.size(); i++)
					ImGui::MenuItem(m_Effects.get(i)->getName().c_str(), NULL, &(m_Effects.get(i)->show_interface));
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		for (unsigned int i = 0; i < m_Resources.size(); i++)
			if (m_Resources.get(i)->show_interface)
				m_Resources.get(i)->displayInterface();
		for (unsigned int i = 0; i < m_Nodes.size(); i++)
			if (m_Nodes.get(i)->show_interface)
				m_Nodes.get(i)->displayInterface();
		for (unsigned int i = 0; i < m_Effects.size(); i++)
			if (m_Effects.get(i)->show_interface)
				m_Effects.get(i)->displayInterface();
		ImGui::EndMainMenuBar();
	}
}