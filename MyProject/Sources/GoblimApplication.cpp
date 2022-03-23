#include "GoblimApplication.h"
#include <iostream>
#include <chrono>
#define M_PI 3.14159265358979323846264338327950288
bool goForward = false;
bool goBackward = false;
bool goLeft = false;
bool goRight = false;
using namespace std;
static void error_callback(int error, const char* description)
{	
	LOG_TRACE << "-------------------------------------------------------------" << std::endl;
    LOG_TRACE << "Error :" << error << "\nDescription: " << description << std::endl;
}

GobLimApplication::GobLimApplication(int width,int height,std::string name)
	:m_width(width),m_height(height),m_title(name)
{
	srand((unsigned int)time(0));
	
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(-1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// FSAA Antialiasing : Possible Performance Killer
	glfwWindowHint(GLFW_REFRESH_RATE, 0);
	glfwSwapInterval(1);
	glfwWindowHint (GLFW_SAMPLES,8);
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window,this);

	try
	{
		glewExperimental = GL_TRUE; 
        glewInit();
        ImGui_ImplGlfwGL3_Init(m_window, false); 							// False because we don't wand Imgui to rewrite the callbacks
		glfwSetMouseButtonCallback(m_window, mouse_callback_glfw);
        glfwSetScrollCallback(m_window, mouse_wheel_callback_glfw);
        glfwSetKeyCallback(m_window, keyboard_callback_glfw);				// Custom Callback Events	
        glfwSetCharCallback(m_window, ImGui_ImplGlfwGL3_CharCallback);
		glfwSetCursorPosCallback(m_window, mousepos_callback_glfw);
		glfwSetFramebufferSizeCallback(m_window, window_resize_callback_glfw);
	}catch (const std::exception & e){cerr << "Error Glew Initalization: " << e.what() <<endl;}

    try
	{
		// Direct engine
		m_engine = new SampleEngine(m_width, m_height);
		// Deferred engine
		//m_engine = new SampleEngine_Deferred(m_width, m_height);
		m_engine->init();
	}catch (const std::exception & e){
		LOG_ERROR << "Error Engine Initalization: "<< e.what() <<endl;
		Logger::getInstance()->show_interface = true;
		std::cout << "paf "<< std::endl;


	}
	
    
    m_scene = Scene::getInstance();

    m_clear_color       = ImColor(120, 120, 120);
	m_display_interface = true;

	m_rotate = m_zoom = m_anim_object = false;		//Trackball
	m_trackball = true;
    m_speed_factor 	= 0.025f;
	m_camera_speed 	= 1.f;
	m_wheel 		= 0.f;

	middle = glm::vec2(m_width/2,m_height/2);
	nMouse = middle;
	m_translate = false;

}

GobLimApplication::~GobLimApplication()
{
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    delete m_engine;
	m_scene->kill();
}

void GobLimApplication::displayOverlay(bool display, float milli, float seconds)
{

	ImGuiStyle& style = ImGui::GetStyle();
	style.ChildWindowRounding = 8.0f;
	style.WindowRounding = 8.0f;
	style.FrameRounding = 8.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    ImGui::SetNextWindowPos(ImVec2(10,20));
    if (!ImGui::Begin("Example: Fixed Overlay", &display, ImVec2(0,0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
        ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    ImGui::Separator();    
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", milli, 1/seconds);
    ImGui::Separator(); 
	static ImVec4 color = ImColor(0.5f, 0.5f, 0.5f, 1.0f);
	ImGui::Text("Clear Color :");
	ImGui::SameLine();
	if (ImGui::ColorButton(color))
		ImGui::OpenPopup("Clear Color");
	if (ImGui::BeginPopup("Clear Color"))
	{
		ImGui::ColorPicker4("", (float*)&color);
		m_engine->setClearColor(glm::vec4(color.x,color.y,color.z,color.w));
		ImGui::EndPopup();
	}
	
	ImGui::Separator(); 
		ImGui::Text("Camera parameters:");
        ImGui::Checkbox(" : Trackball ON/OFF", &m_trackball);
		ImGui::Checkbox(" : Rotate the object", &m_anim_object);
		ImGui::SliderFloat(": Speed Factor",&m_speed_factor, 0.001f, 0.050f);
		ImGui::SliderFloat(": Camera Speed",&m_camera_speed, 0.001f, 1.f);
	ImGui::Separator();
	ImGui::Checkbox(" Draw Lights", &(m_engine->drawLights));
	ImGui::Checkbox(" Draw boxes", &(m_engine->drawBoundingBoxes));
    ImGui::End();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Console"))
		{
			ImGui::MenuItem("Show Console", NULL, &Logger::getInstance()->show_interface);
			ImGui::EndMenu();
		}
		
			
		ImGui::EndMainMenuBar();
	}
	m_engine->displayInterface();
	m_scene->displayInterface();
	if (Logger::getInstance()->show_interface)
		Logger::getInstance()->Draw("Console");
	//ImGui::ShowTestWindow();


}


void GobLimApplication::mousepos_callback_glfw(GLFWwindow* window, double mouseX, double mouseY)
{
	GobLimApplication *g = static_cast<GobLimApplication *>(glfwGetWindowUserPointer(window));
	g->mousepos_event(mouseX, mouseY);
}
void GobLimApplication::keyboard_callback_glfw(GLFWwindow * window, int key, int scancode,int action, int mods)
{
	GobLimApplication *g = static_cast<GobLimApplication *>(glfwGetWindowUserPointer(window));
	g->keyboard_event(key,scancode,action,mods);
}
void GobLimApplication::mouse_callback_glfw(GLFWwindow * window, int button, int action, int mods)
{
	GobLimApplication *g = static_cast<GobLimApplication *>(glfwGetWindowUserPointer(window));
	g->mouse_event(button,action,mods);
}
void GobLimApplication::mouse_wheel_callback_glfw(GLFWwindow * window, double xoffset, double yoffset)
{
	ImGui_ImplGlfwGL3_ScrollCallback(window,xoffset,yoffset);
	GobLimApplication *g = static_cast<GobLimApplication *>(glfwGetWindowUserPointer(window));
	g->mouse_wheel_event(yoffset);
}
void GobLimApplication::window_resize_callback_glfw(GLFWwindow* window, int width, int height)
{
	GobLimApplication *g = static_cast<GobLimApplication *>(glfwGetWindowUserPointer(window));
	
	if (width > 0 && height > 0)
		g->windowSize_event(width,height);

}

void GobLimApplication::windowSize_event( int width, int height)
{
	m_width = width;
	m_height = height;
	if (m_engine != NULL)
		m_engine->onWindowResize(width, height);

}


void GobLimApplication::keyboard_event(int key, int scancode,int action, int mods)
{

	
	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(m_window, GL_TRUE); //Quit Application

	if (action == GLFW_PRESS)
	{
		io.KeysDown[key] = true;
		switch (key)
		{
			case GLFW_KEY_TAB:
				m_display_interface = !m_display_interface;
				break;
			case GLFW_KEY_SPACE:
					m_trackball = !m_trackball;
					break;
				default: break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		io.KeysDown[key] = false;
	}
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		if (action == GLFW_PRESS)
		{
			io.KeysDown[key] = true;
			switch (key)
			{
			case GLFW_KEY_W:
				goForward = true;
				break;
			case GLFW_KEY_S:
				goBackward = true;
				break;
			case GLFW_KEY_A:
				goLeft = true;
				break;
			case GLFW_KEY_D:
				goRight = true;
				break;
			case GLFW_KEY_L:
				Scene::getInstance()->nextCamera();
				break;
			default: break;
			}
		}
		else if (action == GLFW_RELEASE)
		{
			io.KeysDown[key] = false;
			switch (key)
			{
			case GLFW_KEY_W:
				goForward = false;
				break;
			case GLFW_KEY_S:
				goBackward = false;
				break;
			case GLFW_KEY_A:
				goLeft = false;
				break;
			case GLFW_KEY_D:
				goRight = false;
				break;

			default: break;
			}
		}
	}



}


void GobLimApplication::mousepos_event(int x, int y)
{
	if (m_trackball)
	{
		moveFrameTrackball();
	}
}
void GobLimApplication::mouse_event(int button, int action, int mods)
{

	if (!ImGui::GetIO().WantCaptureMouse)
	{
		nMouse.x = ImGui::GetIO().MousePos.x;
		nMouse.y = ImGui::GetIO().MousePos.y;

		oMouse = nMouse;

		if (m_trackball)
		{
			if (action == GLFW_PRESS && button == 0)
			{
				m_rotate = GLFW_PRESS && button == 0;

			}
			else if (action == GLFW_RELEASE && button == 0)
			{
				m_rotate = false;
				oMouse = middle;
			}
			if (action == GLFW_PRESS && button == 1)
			{
				m_translate = true;


			}
			else if (action == GLFW_RELEASE && button == 1)
			{
				m_translate = false;

			}
		}
		



	}
}

void GobLimApplication::mouse_wheel_event(double yoffset)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (m_trackball)
			m_wheel -= yoffset;

		float modif = (yoffset) ? 1.0f : -1.0f;
		glm::vec3 dep = (float)(yoffset)*0.1f*m_scene->camera()->convertDirTo(glm::vec3(0.0, 0.0, -1.0), m_scene->getManipulatedNode()->frame());
		m_scene->getManipulatedNode()->frame()->translate(dep);

	}
	
}

glm::vec3 GobLimApplication::projectOnSphere(glm::vec2 pos)
{
	glm::vec3 v;
	v[0] = pos[0];
	v[1] = pos[1];

	if (v.x*v.x + v.y*v.y <= 0.5f)
		v.z = sqrt(1.0f - (v.x*v.x + v.y*v.y));
	else
		v.z = (0.5f) / sqrt(v.x*v.x + v.y*v.y);
	return v;
}

void GobLimApplication::moveFrameTrackball()
{
	glm::vec2 v, o;
	nMouse.x = ImGui::GetIO().MousePos.x;
	nMouse.y = ImGui::GetIO().MousePos.y;
	
	v = getNormalizedMouseCoord(nMouse);
	o = getNormalizedMouseCoord(oMouse);
	

	if(m_rotate)
	{
		if (length(v - o) > 0.1);
		trackBallFrame(v, o);
	}
	else if (m_translate)
	{
		glm::vec2 dis = v - o;
		glm::vec3 localCenter = m_scene->camera()->convertPtFrom(glm::vec3(0.0, 0.0, 0.0), m_scene->getManipulatedNode()->frame());
		glm::vec3 dir = m_scene->camera()->convertDirTo(glm::vec3(dis.x, dis.y, 0.0), m_scene->getManipulatedNode()->frame());
		m_scene->getManipulatedNode()->frame()->translate(dir*-localCenter.z*0.5f);
		
	}
	oMouse = nMouse;
}
glm::vec2 GobLimApplication::getNormalizedMouseCoord(glm::vec2 m)
{
	glm::vec2 v;
	v.x = (float)m.x / (float)m_width;
	v.y = 1.0f - (float)m.y / (float)m_height;
	v = glm::vec2(2.0f)*v - glm::vec2(1.0f);
	return v;

}
void GobLimApplication::trackBallFrame(glm::vec2  n, glm::vec2 o)
{


	glm::vec3 v1 = projectOnSphere(n);
	glm::vec3 v2 = projectOnSphere(o);
	Frame *f = m_scene->getManipulatedNode()->frame();

	v1 = glm::normalize(v1);
	v2 = glm::normalize(v2);

	glm::vec3 rax = glm::normalize(glm::cross(v2, v1));
	if (!isnan(rax.x))
	{
		float angle = acos(glm::dot(v1, v2));
		 
		rax = glm::normalize(m_scene->camera()->convertDirTo(rax, f));
		f->rotate(rax, (float)(M_PI*angle ));

		

	}
}

void GobLimApplication::drawContents() 					{m_engine->render();}
void GobLimApplication::animate(const int elapsedTime) 	
{
	//if (m_engine)
	m_engine->animate(elapsedTime);
	
	if (goForward) 			m_scene->camera()->translate(glm::vec3(0.0, 0.0, -m_camera_speed));
	else if (goBackward) 	m_scene->camera()->translate(glm::vec3(0.0, 0.0, m_camera_speed));
	if (goRight) 			m_scene->camera()->translate(glm::vec3(m_camera_speed, 0.0, 0.0));
	else if (goLeft) 		m_scene->camera()->translate(glm::vec3(-m_camera_speed, 0.0, 0.0));
	



}
void GobLimApplication::mainLoop()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time, now_time;
	LOG_INFO << "Beginning Main Loop" << std::endl;
    while(!glfwWindowShouldClose(m_window))
    {
        last_time = now_time;	
		now_time = std::chrono::high_resolution_clock::now();
   		auto elapsed_Time 	= now_time - last_time;
		auto elapsed_milli 	= chrono::duration <double, milli> (elapsed_Time).count();
		auto elapsed_second = chrono::duration <double, ratio<1,1> > (elapsed_Time).count();
        
        // RENDERING + EVENT
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        displayOverlay(m_display_interface, elapsed_milli, elapsed_second);

		if(m_anim_object)
			m_scene->getManipulatedNode()->frame()->rotate(glm::vec3(0, 1, 0), 0.25f*m_speed_factor);

		
        // Rendering
      
		
        animate(elapsed_milli);				
		drawContents();

		if(m_display_interface)
       	ImGui::Render();
        
		glfwSwapBuffers(m_window);
    }
    LOG_INFO << "Ending Main Loop" << std::endl;
}
