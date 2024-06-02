#include "Application.h"

namespace Crocodile
{
	Application *Application::s_Instance = nullptr;

	Application::Application(
		const char *name,
		bool resizeable,
		unsigned int width,
		unsigned int height,
		bool useImGui) : window(name, resizeable, width, height), useImGui(useImGui)
	{
		s_Instance = this;
		windowWidth = window.getViewportWidth();
		windowHeight = window.getViewportHeight();
		init();
		loadShaders();
		loadTextures();
		postProcessing = new graphics::PostProcessing(
			resourceManager.shaderManager.getShader("postprocessing"),
			window.getWidth(),
			window.getHeight()	
		);
		scene2d = new s2d::Scene(&window, &resourceManager);
		scene3d = new s3d::Scene(&window, &resourceManager);
	}

	Application::~Application()
	{
		delete postProcessing;
		delete scene2d;
		delete scene3d;
	}

	void Application::run()
	{
#ifdef CROCODILE_EMSCRIPTEN
		std::function<void()> mainLoop = [&]()
		{
			clock.tick(60);
			window.beginRender();
			update(clock.deltaTime);
			scene2d->update(clock.deltaTime);
			render();
			window.endRender();
		};
		emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
		while (!window.closed() && running)
		{
			clock.tick();
			window.beginRender();
			if ((window.getViewportWidth() != windowWidth) || (window.getViewportHeight() != windowHeight))
				resize();		
			if (useImGui)
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
			}
			update(clock.deltaTime);
			fixedUpdate(clock.deltaTime);
			scene3d->update(clock.deltaTime);
			scene2d->update(clock.deltaTime);
			if (enablePostprocessing)
				postProcessing->update(clock.deltaTime);
			render();
			if (useImGui)
			{
				renderImGui();
				ImGuiIO &io = ImGui::GetIO();
				io.DisplaySize = ImVec2((float)window.getWidth(), (float)window.getHeight());
				io.FontGlobalScale = fontImGuiScale;
				mouseOnImGuiWindow = io.WantCaptureMouse;
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
			window.endRender();
		}
#endif
	}

	void Application::render()
	{
		// start postprocessing scene capture
		if (enablePostprocessing)
			postProcessing->beginRender();
		scene3d->render();
		scene2d->render();
		// finish and render postprocessing
		if (enablePostprocessing)
		{
			postProcessing->endRender();
			postProcessing->render((float)glfwGetTime());
		}
	}

	void Application::resize()
	{
		windowWidth = window.getViewportWidth();
		windowHeight = window.getViewportHeight();
		postProcessing->resize(windowWidth, windowHeight);
		scene3d->resize();
	}

	void Application::setCurrentScene2d(s2d::Scene *scene)
	{
		delete this->scene2d;
		this->scene2d = scene;
	}

	void Application::setCurrentScene3d(s3d::Scene *scene)
	{
		delete this->scene3d;
		this->scene3d = scene;
	}

	void Application::init() const
	{

#ifndef CROCODILE_EMSCRIPTEN
		if (useImGui)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO &io = ImGui::GetIO();
			(void)io;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			// ImGui::CustomStyle();
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 330");
		}
		glewInit();
#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// NOTE we might want to disable this for 2d games for correct render ordering
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
    	glEnable(GL_CLIP_DISTANCE0);
	}

	void Application::loadShaders()
	{
		// 2d shaders
#ifdef CROCODILE_EMSCRIPTEN
		resourceManager.shaderManager.addShader("sprite", "assets/shaders/s2d/webgl/sprite_vertex.vs", "assets/shaders/s2d/webgl/sprite_fragment.fs");
		resourceManager.shaderManager.addShader("particle", "assets/shaders/s2d/webgl/particles_vertex.vs", "assets/shaders/s2d/webgl/particles_fragment.fs");
		resourceManager.shaderManager.addShader("text", "assets/shaders/s2d/webgl/text_vertex.vs", "assets/shaders/s2d/webgl/text_fragment.fs");
		resourceManager.shaderManager.addShader("postprocessing", "assets/shaders/s2d/webgl/postprocessing_vertex.vs", "assets/shaders/s2d/webgl/postprocessing_fragment.fs");
		resourceManager.shaderManager.addShader("grid", "assets/shaders/s2d/webgl/grid_vertex.vs", "assets/shaders/s2d/webgl/grid_fragment.fs");
		resourceManager.shaderManager.addShader("line", "assets/shaders/s2d/webgl/line_vertex.vs", "assets/shaders/s2d/webgl/line_fragment.fs");
		resourceManager.shaderManager.addShader("circle", "assets/shaders/s2d/webgl/circle_vertex.vs", "assets/shaders/s2d/webgl/circle_fragment.fs");
		resourceManager.shaderManager.addShader("batch_sprite", "assets/shaders/s2d/webgl/batch_sprite_vertex.vs", "assets/shaders/s2d/webgl/batch_sprite_fragment.fs");

#else
		resourceManager.shaderManager.addShader("sprite", "assets/shaders/s2d/opengl/sprite_vertex.vs", "assets/shaders/s2d/opengl/sprite_fragment.fs");
		resourceManager.shaderManager.addShader("particle", "assets/shaders/s2d/opengl/particles_vertex.vs", "assets/shaders/s2d/opengl/particles_fragment.fs");
		resourceManager.shaderManager.addShader("text", "assets/shaders/s2d/opengl/text_vertex.vs", "assets/shaders/s2d/opengl/text_fragment.fs");
		resourceManager.shaderManager.addShader("postprocessing", "assets/shaders/s2d/opengl/postprocessing_vertex.vs", "assets/shaders/s2d/opengl/postprocessing_fragment.fs");
		resourceManager.shaderManager.addShader("grid", "assets/shaders/s2d/opengl/grid_vertex.vs", "assets/shaders/s2d/opengl/grid_fragment.fs");
		resourceManager.shaderManager.addShader("line", "assets/shaders/s2d/opengl/line_vertex.vs", "assets/shaders/s2d/opengl/line_fragment.fs");
		resourceManager.shaderManager.addShader("circle", "assets/shaders/s2d/opengl/circle_vertex.vs", "assets/shaders/s2d/opengl/circle_fragment.fs");
		resourceManager.shaderManager.addShader("batch_sprite", "assets/shaders/s2d/opengl/batch_sprite_vertex.vs", "assets/shaders/s2d/opengl/batch_sprite_fragment.fs");
#endif
		// 3d shaders
		resourceManager.shaderManager.addShader("shader", "assets/shaders/s3d/opengl/shader.vs", "assets/shaders/s3d/opengl/shader.fs");
		resourceManager.shaderManager.addShader("surface_shader", "assets/shaders/s3d/opengl/surface_shader.vs", "assets/shaders/s3d/opengl/surface_shader.fs");
	}

	void Application::loadTextures()
	{
		resourceManager.loadTexture("assets/textures/distortion_texture.png", "dudv", true);
		resourceManager.loadTexture("assets/textures/normal_map.png", "normal_map", true);
	}

	void Application::loadAudio()
	{
	}

}