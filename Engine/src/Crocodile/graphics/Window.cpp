#include "Window.h"

#include "Crocodile/Application.h"
#include "Crocodile/utils/Logger.h"

namespace Crocodile
{
	namespace graphics
	{

		// callbacks
		void window_resize(GLFWwindow *window, int width, int height);
		void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
		void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
		void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

		bool Window::m_keys[MAX_KEYS];
		bool Window::m_buttons[MAX_BUTTONS];
		int Window::m_scroll = 0;

		Window::Window(const char *name, bool resizeable, unsigned int width, unsigned int height) :
			m_width(width), m_height(height), m_initialWidth(width), m_initialHeight(height)
		{
			m_name = name;
			m_resizeable = resizeable;
			m_scale = {(float)m_width / m_initialWidth, (float)m_height / m_initialHeight};
			if (!init())
				glfwTerminate();

			for (int i = 0; i < MAX_KEYS; i++)
			{
				m_keys[i] = false;
			}
			for (int i = 0; i < MAX_BUTTONS; i++)
			{
				m_buttons[i] = false;
			}
		}

		Window::~Window()
		{
			glfwTerminate();
		}

		bool Window::init()
		{
			if (!initGLFW() || !initWindow() || !initGLEW())
				return false;
			initCallbacks();
			LOG(INFO, "OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));
			return true;
		}

		bool Window::initGLFW()
		{
			if (!glfwInit())
			{
				LOG(ERROR, "Failed to initialise GLFW");
				return false;
			}
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			LOG(INFO, "Initialised GLFW");
			return true;
		}

		bool Window::initWindow()
		{
			// window hints
			if (m_resizeable)
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			else
				glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
			// create window
			m_window = glfwCreateWindow(m_width, m_height, m_name, NULL, NULL);
			if (!m_window) {
				LOG(ERROR, "Failed to create GLFW window");
				return false;
			}
			glfwMakeContextCurrent(m_window);
			glfwSetWindowUserPointer(m_window, this);
			LOG(INFO, "Initialised window");
			return true;
		}

		bool Window::initGLEW()
		{
			GLenum err = glewInit();
			if (err != GLEW_OK) {
				LOG(ERROR, "Failed to initialise GLEW");
				return false;
			}
			LOG(INFO, "Initialised GLEW");
			return true;
		}

		void Window::initCallbacks()
		{
			glfwMakeContextCurrent(m_window);
			glfwSetWindowSizeCallback(m_window, window_resize);
			glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
			glfwSetKeyCallback(m_window, key_callback);
			glfwSetMouseButtonCallback(m_window, mouse_button_callback);
			glfwSetScrollCallback(m_window, scroll_callback);
		}

		bool Window::closed() const
		{
			return glfwWindowShouldClose(m_window) == 1;
		}

		void Window::beginRender()
		{
			// clear buffers
			glClearColor(m_color.x, m_color.y, m_color.z, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwGetFramebufferSize(m_window, &m_width, &m_height);
		}

		void Window::endRender()
		{
			glFlush();
			glfwPollEvents();
		}

		bool Window::isKeyPressed(unsigned int keycode)
		{
			if (keycode >= MAX_KEYS)
				return false;
			return m_keys[keycode];
		}

		bool Window::isButtonPressed(unsigned int button)
		{
			if (button >= MAX_BUTTONS)
				return false;
			return m_buttons[button];
		}

		glm::vec2 Window::getMouseScreenPosition()
		{
			double x, y;
			glfwGetCursorPos(m_window, &x, &y);
			return glm::vec2(x, y);
		}

		void Window::setBackgroundColor(glm::vec3 color)
		{
			m_color = color;
		}

		void window_resize(GLFWwindow *window, int width, int height)
		{
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->m_width = width;
			win->m_height = height;
			win->m_scale = {(float)width / win->m_initialWidth, (float)height / win->m_initialHeight};
			glViewport(0, 0, width, height);
		}

		void framebuffer_size_callback(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->m_keys[key] = action != GLFW_RELEASE;
		}

		void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
		{
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->m_buttons[button] = action != GLFW_RELEASE;
		}

		void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
		{
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->m_scroll += (int)yoffset;
		}

	}
}