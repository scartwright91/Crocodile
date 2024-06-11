#pragma once

#ifdef CROCODILE_EMSCRIPTEN
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#endif
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core.h"

namespace Crocodile
{
	namespace graphics
	{

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

		// callbacks
		void window_resize(GLFWwindow *window, int width, int height);
		void framebuffer_size_callback(GLFWwindow *window, int width, int height);
		void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
		void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
		void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

		class CROCODILE_API Window
		{

		public:
			int m_width, m_height;
			const int m_initialWidth, m_initialHeight;
			glm::vec2 m_scale;

		private:
			bool m_resizeable;
			const char* m_name;
			GLFWwindow* m_window;

		public:
			Window(const char *name, bool resizeable, unsigned int width, unsigned int height);
			~Window();

			bool closed() const;

			void beginRender();
			void endRender();

			inline int getWidth() const { return m_width; }
			inline int getHeight() const { return m_height; }
			inline glm::vec2 getScale() const { return m_scale; }
			inline int getScroll() const { return m_scroll; }
			inline GLFWwindow *getWindow() const { return m_window; }

			static bool isKeyPressed(unsigned int keycode);
			static bool isButtonPressed(unsigned int button);
			glm::vec2 getMouseScreenPosition();

			void setBackgroundColor(glm::vec3 color);

			static int m_scroll;
			static bool m_keys[MAX_KEYS];
			static bool m_buttons[MAX_BUTTONS];

		private:
			bool init();
			bool initGLFW();
			bool initWindow();
			bool initGLEW();
			void initCallbacks();
			glm::vec3 m_color = glm::vec3(0.f);
		};
	}
}
