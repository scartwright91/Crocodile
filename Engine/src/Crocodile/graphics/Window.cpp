#include "Window.h"

#include "Crocodile/Application.h"

namespace Crocodile
{
	namespace graphics
	{

		// callbacks
		void window_resize(GLFWwindow *window, int width, int height);
		void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
		void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
		void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

		bool Window::keys[MAX_KEYS];
		bool Window::buttons[MAX_BUTTONS];
		int Window::scroll = 0;

		Window::Window(const char *name, bool resizeable, unsigned int width, unsigned int height) : width(width), height(height), initialWidth(width), initialHeight(height)
		{
			this->name = name;
			this->resizeable = resizeable;
			if (!init())
				glfwTerminate();
			else
				std::cout << "Created game window" << std::endl;

			for (int i = 0; i < MAX_KEYS; i++)
			{
				keys[i] = false;
			}
			for (int i = 0; i < MAX_BUTTONS; i++)
			{
				buttons[i] = false;
			}
		}

		Window::~Window()
		{
			glfwTerminate();
		}

		bool Window::init()
		{
			// init
			if (!glfwInit())
			{
				std::cout << "Failed to initialise GLFW" << std::endl;
				return false;
			}
			else
			{
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				std::cout << "Initialised GLFW" << std::endl;
			}

			// window resizing
			if (!resizeable)
				glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			else
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

			// create window
			int count;
			GLFWmonitor **monitors = glfwGetMonitors(&count);
			window = glfwCreateWindow(width, height, name, NULL, NULL);

			if (!window)
			{
				std::cout << "Failed to create GLFW window" << std::endl;
				return false;
			}

			updateViewport();

			// joystick
			int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
			if (present)
				std::cout << "Joystick detected" << std::endl;

			// set callbacks
			glfwMakeContextCurrent(window);
			glfwSetWindowSizeCallback(window, window_resize);
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			glfwSetKeyCallback(window, key_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwSetScrollCallback(window, scroll_callback);

			std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

			return true;
		}

		void Window::updateViewport()
		{
			// viewport sizing
			glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
			glViewport(0, 0, viewportWidth, viewportHeight);
			viewportScale = glm::vec2(
				(float)viewportWidth / initialWidth,
				(float)viewportHeight / initialHeight);
		}

		bool Window::closed() const
		{
			return glfwWindowShouldClose(window) == 1;
		}

		void Window::beginRender()
		{
			// viewport sizing
			updateViewport();
			// clear buffers
			glClearColor(color.x, color.y, color.z, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwGetFramebufferSize(window, &width, &height);
		}

		void Window::endRender()
		{
			// glfwSwapBuffers(window);
			glFlush();
			glfwPollEvents();
		}

		bool Window::isKeyPressed(unsigned int keycode)
		{
			if (keycode >= MAX_KEYS)
				return false;
			return keys[keycode];
		}

		bool Window::isButtonPressed(unsigned int button)
		{
			if (button >= MAX_BUTTONS)
				return false;
			return buttons[button];
		}

		glm::vec2 Window::getMouseScreenPosition()
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			return glm::vec2(x, y) / viewportScale;
		}

		void Window::setBackgroundColor(glm::vec3 color)
		{
			this->color = color;
		}

		void window_resize(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		void framebuffer_size_callback(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->keys[key] = action != GLFW_RELEASE;
		}

		void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
		{
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->buttons[button] = action != GLFW_RELEASE;
		}

		void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
		{
			Window *win = (Window *)glfwGetWindowUserPointer(window);
			win->scroll += (int)yoffset;
		}

	}
}