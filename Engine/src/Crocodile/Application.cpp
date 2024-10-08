#include "Application.h"

namespace Crocodile
{
	Application *Application::s_Instance = nullptr;

	Application::Application(
		const char *name,
		bool resizeable,
		unsigned int width,
		unsigned int height,
		bool useImGui,
		bool useLua
	) : m_window(name, resizeable, width, height), m_useImGui(useImGui)
	{
		s_Instance = this;
		m_windowWidth = m_window.getWidth();
		m_windowHeight = m_window.getHeight();
		init();
		loadShaders();
		loadTextures();
		m_postProcessing = new graphics::PostProcessing(
			m_resourceManager.m_shaderManager.getShader("postprocessing"),
			m_window.getWidth(),
			m_window.getHeight()	
		);
		m_scene2d = new s2d::Scene(&m_window, &m_resourceManager);
		m_scene3d = new s3d::Scene(&m_window, &m_resourceManager);

		m_scriptSystem.enabled = useLua;
		if (useLua)
		{
			// Lua bindings
			m_scriptSystem.lua.set_function("is_key_pressed", &m_window.isKeyPressed);
			m_scriptSystem.lua.set_function("is_mouse_pressed", &m_window.isButtonPressed);
			m_scene2d->addLuaBindings(m_scriptSystem.lua);
			m_resourceManager.addLuaBindings(m_scriptSystem.lua);

			// add scene to lua
			m_scriptSystem.lua["scene2d"] = m_scene2d;
			m_scriptSystem.lua["resource_manager"] = m_resourceManager;

			// load main script
			m_scriptSystem.loadMainScript();
		}

	}

	Application::~Application()
	{
		delete m_postProcessing;
		delete m_scene2d;
		delete m_scene3d;
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
		while (!m_window.closed() && m_running)
		{
			m_clock.tick();
			m_window.beginRender();
			if ((m_window.getWidth() != m_windowWidth) || (m_window.getHeight() != m_windowHeight))
				resize();		
			if (m_useImGui)
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
			}
			m_resourceManager.update();
			m_scriptSystem.update();
			update(m_clock.getDeltaTime());
			m_scene3d->update(m_clock.getDeltaTime());
			m_scene2d->update(m_clock.getDeltaTime());
			if (m_enablePostprocessing)
				m_postProcessing->update(m_clock.getDeltaTime());
			render();
			if (m_useImGui)
			{
				renderImGui();
				ImGuiIO &io = ImGui::GetIO();
				io.DisplaySize = ImVec2((float)m_window.getWidth(), (float)m_window.getHeight());
				io.FontGlobalScale = m_fontImGuiScale;
				m_mouseOnImGuiWindow = io.WantCaptureMouse;
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
			m_window.endRender();
		}
#endif
	}

	void Application::render()
	{
		// start postprocessing scene capture
		if (m_enablePostprocessing)
			m_postProcessing->beginRender();
		glEnable(GL_DEPTH_TEST);
		m_scene3d->render();
		glDisable(GL_DEPTH_TEST);
		m_scene2d->render();
		// finish and render postprocessing
		if (m_enablePostprocessing)
		{
			m_postProcessing->endRender();
			m_postProcessing->render((float)glfwGetTime());
		}
	}

	void Application::resize()
	{
		m_windowWidth = m_window.getWidth();
		m_windowHeight = m_window.getHeight();
		m_postProcessing->resize(m_windowWidth, m_windowHeight);
		m_scene2d->scaleScene();
		m_scene3d->resize();
	}

	void Application::setCurrentScene2d(s2d::Scene *scene)
	{
		delete this->m_scene2d;
		this->m_scene2d = scene;
	}

	void Application::setCurrentScene3d(s3d::Scene *scene)
	{
		delete this->m_scene3d;
		this->m_scene3d = scene;
	}

	void Application::init() const
	{

#ifndef CROCODILE_EMSCRIPTEN
		if (m_useImGui)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO &io = ImGui::GetIO();
			(void)io;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			// ImGui::CustomStyle();
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(m_window.getWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 330");
		}
		glewInit();
#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
    	glEnable(GL_CLIP_DISTANCE0);
	}

	void Application::loadShaders()
	{
		// 2d shaders
#ifdef CROCODILE_EMSCRIPTEN
		m_resourceManager.m_shaderManager.addShader("sprite", "assets/shaders/s2d/webgl/sprite_vertex.vs", "assets/shaders/s2d/webgl/sprite_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("particle", "assets/shaders/s2d/webgl/particles_vertex.vs", "assets/shaders/s2d/webgl/particles_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("text", "assets/shaders/s2d/webgl/text_vertex.vs", "assets/shaders/s2d/webgl/text_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("postprocessing", "assets/shaders/s2d/webgl/postprocessing_vertex.vs", "assets/shaders/s2d/webgl/postprocessing_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("grid", "assets/shaders/s2d/webgl/grid_vertex.vs", "assets/shaders/s2d/webgl/grid_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("line", "assets/shaders/s2d/webgl/line_vertex.vs", "assets/shaders/s2d/webgl/line_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("circle", "assets/shaders/s2d/webgl/circle_vertex.vs", "assets/shaders/s2d/webgl/circle_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("batch_sprite", "assets/shaders/s2d/webgl/batch_sprite_vertex.vs", "assets/shaders/s2d/webgl/batch_sprite_fragment.fs");

#else
		m_resourceManager.m_shaderManager.addShader("sprite", "assets/shaders/s2d/opengl/sprite_vertex.vs", "assets/shaders/s2d/opengl/sprite_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("particle", "assets/shaders/s2d/opengl/particles_vertex.vs", "assets/shaders/s2d/opengl/particles_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("text", "assets/shaders/s2d/opengl/text_vertex.vs", "assets/shaders/s2d/opengl/text_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("postprocessing", "assets/shaders/s2d/opengl/postprocessing_vertex.vs", "assets/shaders/s2d/opengl/postprocessing_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("grid", "assets/shaders/s2d/opengl/grid_vertex.vs", "assets/shaders/s2d/opengl/grid_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("line", "assets/shaders/s2d/opengl/line_vertex.vs", "assets/shaders/s2d/opengl/line_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("circle", "assets/shaders/s2d/opengl/circle_vertex.vs", "assets/shaders/s2d/opengl/circle_fragment.fs");
		m_resourceManager.m_shaderManager.addShader("batch_sprite", "assets/shaders/s2d/opengl/batch_sprite_vertex.vs", "assets/shaders/s2d/opengl/batch_sprite_fragment.fs");
#endif
		// 3d shaders
		m_resourceManager.m_shaderManager.addShader("shader", "assets/shaders/s3d/opengl/shader.vs", "assets/shaders/s3d/opengl/shader.fs");
		m_resourceManager.m_shaderManager.addShader("surface_shader", "assets/shaders/s3d/opengl/surface_shader.vs", "assets/shaders/s3d/opengl/surface_shader.fs");
	}

	void Application::loadTextures()
	{
		m_resourceManager.loadTexture("assets/textures/dudv.png", "dudv", true);
		m_resourceManager.loadTexture("assets/textures/normal_map.png", "normal_map", true);
	}

	void Application::loadAudio()
	{
	}

}