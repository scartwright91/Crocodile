#include "Application.h"

namespace Crocodile
{
	Application *Application::s_Instance = nullptr;

	Application::Application(
		const char *name,
		bool resizeable,
		unsigned int width,
		unsigned height) : window(name, resizeable, width, height)
	{
		s_Instance = this;

		init();
		loadShaders();
		loadTextures();
		scene = new s2d::Scene(&window, &resourceManager);
	}

	Application::~Application()
	{
		delete scene;
	}

	void Application::run()
	{
#ifdef CROCODILE_EMSCRIPTEN
		std::function<void()> mainLoop = [&]()
		{
			clock.tick(60);
			window.beginRender();
			update(clock.deltaTime);
			scene->update(clock.deltaTime);
			render();
			window.endRender();
		};
		emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
		while (!window.closed())
		{
			clock.tick(60);
			window.beginRender();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			update(clock.deltaTime);
			scene->update(clock.deltaTime);
			render();
			renderImGui();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			window.endRender();
		}
#endif
	}

	void Application::render()
	{
		scene->render();
	}

	void Application::setCurrentScene(s2d::Scene *scene)
	{
		delete this->scene;
		this->scene = scene;
	}

	void Application::init() const
	{

#ifndef CROCODILE_EMSCRIPTEN
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
		glewInit();
#endif
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Application::loadShaders()
	{
#ifdef CROCODILE_EMSCRIPTEN
		resourceManager.loadShader("assets/shaders/webgl/sprite_vertex.vs", "assets/shaders/webgl/sprite_fragment.fs", "sprite");
		resourceManager.loadShader("assets/shaders/webgl/particle_vertex.vs", "assets/shaders/webgl/particle_fragment.fs", "particle");
		resourceManager.loadShader("assets/shaders/webgl/text_vertex.vs", "assets/shaders/webgl/text_fragment.fs", "text");
		resourceManager.loadShader("assets/shaders/webgl/postprocessing_vertex.vs", "assets/shaders/webgl/postprocessing_fragment.fs", "postprocessing");
		resourceManager.loadShader("assets/shaders/webgl/grid_vertex.vs", "assets/shaders/webgl/grid_fragment.fs", "grid");
#else
		resourceManager.loadShader("assets/shaders/opengl/sprite_vertex.vs", "assets/shaders/opengl/sprite_fragment.fs", "sprite");
		resourceManager.loadShader("assets/shaders/opengl/particles_vertex.vs", "assets/shaders/opengl/particles_fragment.fs", "particle");
		resourceManager.loadShader("assets/shaders/opengl/text_vertex.vs", "assets/shaders/opengl/text_fragment.fs", "text");
		resourceManager.loadShader("assets/shaders/opengl/postprocessing_vertex.vs", "assets/shaders/opengl/postprocessing_fragment.fs", "postprocessing");
		resourceManager.loadShader("assets/shaders/opengl/grid_vertex.vs", "assets/shaders/opengl/grid_fragment.fs", "grid");
#endif
	}

	void Application::loadTextures()
	{
		resourceManager.loadTexture("assets/textures/waterDUDV.png", "waterDUDV", true);
	}

	void Application::loadAudio()
	{
	}

}