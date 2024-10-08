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
#include "scripting/ScriptingSystem.h"
#include "s2d/LevelParser.h"

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
			bool useImGui,
			bool useLua
		);
		virtual ~Application();

		ResourceManager m_resourceManager;
		SoundManager m_soundManager;
		scripting::ScriptingSystem m_scriptSystem;

		bool m_running = true;
		graphics::Window m_window;
		graphics::PostProcessing* m_postProcessing = nullptr;

		// 2d
		s2d::Scene *m_scene2d;
		s2d::LevelParser *m_levelParser;

		// 3d
		s3d::Scene *m_scene3d;
		Clock m_clock;

		// imgui variables
		float m_fontImGuiScale = 1.f;
		bool m_mouseOnImGuiWindow = false;
		bool m_enablePostprocessing = true;

	private:
		void loadShaders();
		void loadTextures();
		void loadAudio();

		void resize();

		void init() const;

	public:
		static Application &get() { return *s_Instance; }

		void run();
		void render();
		void setCurrentScene2d(s2d::Scene *scene);
		void setCurrentScene3d(s3d::Scene *scene);
		virtual void update(float dt){};
		virtual void renderImGui(){};

	private:
		static Application *s_Instance;
		bool m_useImGui = false;
		unsigned int m_windowWidth, m_windowHeight;

	};

	// to be defined in client
	Application *CreateApplication();
}
