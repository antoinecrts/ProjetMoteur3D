#ifndef _GOBLIM_APPLICATION
#define _GOBLIM_APPLICATION

#include "Utils/imgui/imgui.h"
#include "Utils/imgui/imgui_impl_glfw_gl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include "SampleEngine.h"
#include "SampleEngine_Deferred.h"
class GobLimApplication
{
    public:
        GobLimApplication(int width = 1024,int height=1024,std::string name="GobLim Engine");
        ~GobLimApplication();

        void mainLoop();
        
        // Display Imgui Overlay
        void displayOverlay(bool display, float milli, float fps);

        // Camera
        void moveFrameTrackball();
        glm::vec3 projectOnSphere(glm::vec2 pos); 
        void trackBallFrame(glm::vec2 newMouse, glm::vec2 oldMouse);
		glm::vec2 getNormalizedMouseCoord(glm::vec2 m);
        // Draw & Animation
        void drawContents();
        void animate(const int elapsedTime);

        //KeyBoard Event
        static void keyboard_callback_glfw(GLFWwindow * window, int key, int scancode,int action, int mods);
        static void mouse_callback_glfw(GLFWwindow * window, int button, int action, int mods);
        static void mouse_wheel_callback_glfw(GLFWwindow * window, double xoffset, double yoffset);
		static void mousepos_callback_glfw(GLFWwindow* win, double mouseX, double mouseY);
		static void window_resize_callback_glfw(GLFWwindow* win, int width, int height);
		void mousepos_event(int x, int y);
        void keyboard_event(int key, int scancode, int action, int mods);
        void mouse_event(int button, int action, int mods);
        void mouse_wheel_event(double yoffset);
		void windowSize_event(int width,int height);

    private:
        GLFWwindow      *   m_window;
        int                 m_height, m_width;
        std::string         m_title;
        
        ImVec4              m_clear_color;
        bool                m_show_overlay;
        bool                m_display_interface;

        //SampleEngine    *   m_engine;
		EngineGL    *   m_engine;
        Scene           *   m_scene;
        bool                m_anim_object;

        // Camera Attributes
        bool                m_trackball, m_firstperson, m_translate;                 // Mode
        bool                mode_on, m_rotate, m_zoom;                  // Trackball operations
        float               m_speed_factor, m_camera_speed, m_wheel;

        float               angleYaw, anglePitch;
        glm::vec3           camPos, camDir;
        glm::vec2           oMouse, nMouse;
        glm::vec2           middle;
        
  
};



#endif