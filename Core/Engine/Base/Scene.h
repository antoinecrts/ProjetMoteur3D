#ifndef _GOBLIM_SCENE_
#define _GOBLIM_SCENE_

#include "Engine/Base/Node.h"
#include "Engine/Base/GeometricModel.h"
#include "Engine/OpenGL/CameraGL.h"
#include "Engine/Base/Material.h"
#include "Engine/Base/Resource.h"
#include "Engine/Base/Effect.h"
#include "Utils/Resource_mgr.hpp"
#include "Utils/Singleton.h"
#include "Engine/OpenGL/ModelGL.h"
#include "Utils/Logger/ImGuiLogger.h"


class Scene : public Singleton<Scene>{
	friend class Singleton<Scene>;
public:
	
	
	Frame* frame();
	CameraGL* camera();
	
	Node* getRoot();
	Node* getNode(std::string name);
	Node* getQuad();

	
	template <class R>  R* getEffect(string a)
	{
		return m_Effects.get<R>(a);
	}

	template <class R>  R* getModel(string a)
	{		
		return m_Models.get<R>(a);
	}
	template <class R> R* getResource(string a)
	{
		return m_Resources.get<R>(a);
	}
	template <class R> R* getNodes(string a)
	{
		return m_Nodes.get<R>(a);
	}
	void releaseNode(string a);
	void releaseNode(Node *n);

	void releaseEffect(string a);
	void releaseEffect(Effect *n);


	void releaseResource(string a);
	void releaseResource(Resource *r);

	void releaseModel(string a);
	void releaseModel(Model *m);

	void nextCamera();
	void useCamera(std::string name);

	void nextManipulatedNode();
	void manipulateNode(std::string name);

	void displayInterface();
	Node* getSceneNode();
	Node* getManipulatedNode();

	CameraGL* getCamera(std::string name);
	

	Resource_mgr<Resource> m_Resources;	
	Resource_mgr<Node> m_Nodes;
	Resource_mgr<Model> m_Models;
	Resource_mgr<Effect> m_Effects;
	Resource_mgr<CameraGL> m_Cameras;
	bool needupdate;
	
private:
	Scene(); 
	~Scene();

	Frame *m_Frame;
	Node* m_Root;
	Node *m_Scene;
	CameraGL* current_Camera;
	Node* current_ManipulatedNode;

};



#endif