#include "Engine/OpenGL/Lighting/LightingModelGL.h"
#include "Engine/OpenGL/Managers/GLUniformBlock.h"
#include "Engine/Base/Scene.h"


void printLightBuffer(LightBuffer* p)
{
	LOG_TRACE << "camPos = (" << p->camPos.x << ";" << p->camPos.y << ";" << p->camPos.z << ";" << p->camPos.w << ")" << std::endl;
	LOG_TRACE << "nbLights = (" << p->nbLights.x << ";" << p->nbLights.y << ";" << p->nbLights.z << ";" << p->nbLights.w << ")" << std::endl;
	for (int i = 0; i < p->nbLights.x; i++)
	{
		LOG_TRACE << "Light=[" << i << "] = { " << std::endl;
		printLight(&(p->Lights[i]));
		LOG_TRACE <<  "}" << std::endl;
	}
}

LightingModelGL::LightingModelGL(std::string name):
	LightingModel(name)
{
	m_Buffer = new GPUShaderStorageBuffer("LightingModel");
	lightParamsSize = sizeof(LightBuffer) + 100 * sizeof(Light);
	m_Buffer->create(lightParamsSize, GL_DYNAMIC_COPY);
	lightParams = (LightBuffer*) malloc(sizeof(LightBuffer)+100 * sizeof(Light));
	m_Buffer->setBindingPoint(LIGHTING_SSBO_BINDING);
}

LightingModelGL::LightingModelGL(std::string name, Node* root)
	:LightingModel(name)
{
	rootNode = root;
	this->collect(root);

	m_Buffer = new GPUBuffer("LightingModel");
	lightParamsSize = sizeof(LightBuffer)+100 * sizeof(Light);
	m_Buffer->create(lightParamsSize, GL_SHADER_STORAGE_BUFFER,GL_DYNAMIC_COPY);
	m_Buffer->setBindingPoint(LIGHTING_SSBO_BINDING);
	
	lightParams = (LightBuffer*)malloc(lightParamsSize);
	
	//std::cout << "Monitoring size of elements :" << std::endl;
	//std::cout << "lightParams Theorical size = " << (sizeof(LightBuffer)+collector->nodes.size() * sizeof(Light)) << std::endl;
	//std::cout << "NB lights = " << collector->nodes.size() << std::endl;
	//std::cout << "Sizeof light = " << sizeof(Light) << std::endl;
	//std::cout << "Sizeof vec4 = " << sizeof(glm::vec4) << std::endl;
	//std::cout << "Sizeof ivec4 = " << sizeof(glm::ivec4) << std::endl;
	//int sizeOfLightParams = sizeof(Light)*collector->nodes.size() + sizeof(glm::vec4) + sizeof(glm::ivec4);
	//std::cout << "Test lightbuffer : " << sizeof((*lightParams)) << " vs theorical lightbuffer : " << sizeOfLightParams << std::endl;

}

LightingModelGL::~LightingModelGL()
{
	delete m_Buffer;
}
GPUBuffer* LightingModelGL::getBuffer()
{
	return m_Buffer;
}

void LightingModelGL::setWindowSize(glm::vec2 size)
{
	wSize = size;
	needUpdate = true;
}

void LightingModelGL::update(bool forceUpdate)
{
	LightingModel::update();
	needUpdate = forceUpdate;

	//m_Buffer->bind();
	//
	//if (Scene::getInstance()->camera()->needUpdate())
	//{
	//	glm::vec4 posCam = glm::vec4(Scene::getInstance()->camera()->convertPtTo(glm::vec3(0.0,0.0,0.0),Scene::getInstance()->getRoot()->frame()),1.0);
	//	m_Buffer->Set(0,sizeof(glm::vec4), &posCam);
	//	
	//}
	//
	//
	//if (needUpdate )
	//{				
	//	int size =  (int)collector->nodes.size();
	//	m_Buffer->Set(sizeof(glm::vec4),sizeof(glm::vec4), &size);
	//	
	//	int offset = (int) 2.0*sizeof(glm::vec4);	
	//	
	//	Light* ptr = (Light*)m_Buffer->mapRange( offset, sizeof(Light)*size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	//	int i = 0;
	//
	//	// iterators
	//	for (vector< Node* >::iterator it = collector->nodes.begin(); it != collector->nodes.end(); ++it)
	//	{
	//		LightNode *lnode = dynamic_cast< LightNode* > ((*it));
	//		ptr[i] = lnode->getParams();
	//		i++;
	//	}
	//	
	//	m_Buffer->unMap();
	//	needUpdate = false;
	//	
	//
	//}


	// Variation : mapping buffer to structure
	bool camNeedUpdate = Scene::getInstance()->camera()->needUpdate();
	if (camNeedUpdate)
	{
		lightParams->camPos = glm::vec4(Scene::getInstance()->camera()->convertPtTo(glm::vec3(0.0, 0.0, 0.0), Scene::getInstance()->getRoot()->frame()), 1.0);
	}
	if (needUpdate)
	{
		lightParams->nbLights = glm::ivec4((int)collector->nodes.size(), 0, 0, 0);

		// iterators
		int i = 0;
		for (vector< Node* >::iterator it = collector->nodes.begin(); it != collector->nodes.end(); ++it)
		{
			LightNode *lnode = dynamic_cast< LightNode* > ((*it));
			lightParams->Lights[i] = lnode->getParams();
			i++;
		}
	}
	if (needUpdate || camNeedUpdate)
	{
		
		m_Buffer->bind();
		LightBuffer* ptr = (LightBuffer*)m_Buffer->map(GL_WRITE_ONLY);
		if (ptr != NULL)
		{
			memcpy(ptr, lightParams, lightParamsSize);
			m_Buffer->unMap();
		}
		else LOG_ERROR<< "Failed to update lightbuffer" << std::endl;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		needUpdate = false;

	}
	m_Buffer->bind();
	

}

void LightingModelGL::bind(int location)
{
	m_Buffer->bind(location);
}

void LightingModelGL::renderLights()
{
	for (unsigned int i = 0; i < collector->nodes.size(); i++) 
		collector->nodes[i]->render();
}

void LightingModelGL::displayInterface(const char* title)
{
	if (!ImGui::Begin(title, &show_interface))
	{
		ImGui::End();
		return;
	}
	if (ImGui::Button("Add New Light"))
		ImGui::OpenPopup("Add Light");

	bool addingLight = false;
	char buf[32] = "Light";
	if (ImGui::BeginPopupModal("Add Light", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Enter a Name for the new Light");
		ImGui::Separator();
		ImGui::InputText("", buf, 32);

		if (ImGui::Button("OK", ImVec2(120, 0))) 
		{ 
			addingLight = true;
			ImGui::CloseCurrentPopup(); 
		}
			ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { 
			ImGui::CloseCurrentPopup(); 
		}
			ImGui::EndPopup();
	}
	if (addingLight)
	{
		LOG_INFO << "Adding Light : " << std::string(buf) << std::endl;
		LightNode *lnode = Scene::getInstance()->getNodes<LightNode>(buf);
		Scene::getInstance()->getSceneNode()->adopt(lnode);
		collector->nodes.push_back(lnode);
	}
	

	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	for (unsigned int i = 0; i < getNodes().size(); i++)
	{
		LightNode* lnode = static_cast <LightNode*>(getNodes()[i]);
		lnode->displayInterface();
		ImGui::SameLine();
	}
	needUpdate = true;
	ImGui::PopStyleVar();
	ImGui::End();

}