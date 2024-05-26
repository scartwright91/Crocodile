#pragma once

#ifdef CROCODILE_EMSCRIPTEN
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#else
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#endif

#include "Core.h"
#include "Clock.h"
#include "graphics/Window.h"
#include "graphics/PostProcessing.h"
#include "ResourceManager.h"
#include "audio/SoundManager.h"

#include "s2d/Scene.h"
#include "s3d/Scene.h"

#ifdef CROCODILE_EMSCRIPTEN
#include <functional>
static void dispatch_main(void *fn)
{
	std::function<void()> *func = (std::function<void()> *)fn;
	(*func)();
}
#endif

namespace Crocodile
{
	class CROCODILE_API Application
	{

	public:
		Application(
			const char *name,
			bool resizeable,
			unsigned int width,
			unsigned int height,
			bool useImGui);
		virtual ~Application();

		ResourceManager resourceManager;
		SoundManager soundManager;

		bool running = true;
		graphics::Window window;
		graphics::PostProcessing* postProcessing = nullptr;
		s2d::Scene *scene2d;
		s3d::Scene *scene3d;
		Clock clock;

		// imgui variables
		float fontImGuiScale = 1.f;
		bool mouseOnImGuiWindow = false;
		bool enablePostprocessing = true;

	private:
		void loadShaders();
		void loadTextures();
		void loadAudio();
		void init() const;

	public:
		static Application &get() { return *s_Instance; }

		void run();
		void render();
		void setCurrentScene2d(s2d::Scene *scene);
		void setCurrentScene3d(s3d::Scene *scene);
		virtual void update(float dt){};
		virtual void fixedUpdate(float dt){};
		virtual void renderImGui(){};

	private:
		static Application *s_Instance;
		bool useImGui = false;

	};

	// to be defined in client
	Application *CreateApplication();
}
