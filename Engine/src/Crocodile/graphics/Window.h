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

		private:
			bool resizeable;
			int width, height;
			int viewportWidth, viewportHeight;
			const int initialWidth, initialHeight;
			glm::vec2 viewportScale;
			const char *name;
			GLFWwindow *window;

		public:
			Window(const char *name, bool resizeable, unsigned int width, unsigned int height);
			~Window();

			bool closed() const;

			void beginRender();
			void endRender();

			inline int getWidth() const { return width; }
			inline int getHeight() const { return height; }
			inline int getViewportWidth() const { return viewportWidth; }
			inline int getViewportHeight() const { return viewportHeight; }
			inline glm::vec2 getViewportScale() const { return viewportScale; }
			inline glm::vec2 getScroll() const { return scroll; }
			inline GLFWwindow *getWindow() const { return window; }

			static bool isKeyPressed(unsigned int keycode);
			static bool isButtonPressed(unsigned int button);
			glm::vec2 getMouseScreenPosition();

			void setBackgroundColor(glm::vec3 color);

			static glm::vec2 scroll;
			static bool keys[MAX_KEYS];
			static bool buttons[MAX_BUTTONS];

		private:
			bool init();
			void updateViewport();
			glm::vec3 color = glm::vec3(0.f);
		};
	}
}
