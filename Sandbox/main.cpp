
#include "Crocodile.h"
#include "Crocodile/s2d/Particles.h"
#include "Crocodile/s3d/Surface.h"

#include "LDtkLoader/Project.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    float timer = 0.0f;
    float elapsed = 0.0f;

    // mouse
    float lastX = 0.0f;
    float lastY = 0.0f;

    s2d::Text* fps = new s2d::Text();

    Sandbox() : Crocodile::Application("Sandbox", false, 1280, 720, false)
    {
        init();
    }

    ~Sandbox()
    {

    }

    void update(float dt)
    {
        elapsed += dt;
        fps->text = std::to_string(clock.getFPS());
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
            running = false;

        // scene3d->camera->position.z += dt;
        processCommands(dt);

    }

    void init()
    {
        window.setBackgroundColor(glm::vec3(0.02f, 0.13f, 0.22f));
        scene2d->enablePostprocessing = false;

        s2d::Layer* layer = new s2d::Layer("hud");
        layer->applyCamera = false;
        scene2d->layerStack->addLayer(layer);

        fps->color = glm::vec3(1.f);
        scene2d->addObject(fps, "hud");

        // create cubes
        std::vector<glm::vec3> cubes = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        for (glm::vec3 cube : cubes)
        {
            s3d::Object* obj = new s3d::Object(cube);
            scene3d->addObject(obj);
        }

        s3d::Surface* surface = new s3d::Surface("assets/textures/iceland_heightmap.png", resourceManager.getShader("surface_shader"));
        scene3d->surfaces.push_back(surface);

    }

    void processCommands(float dt)
    {
        if (window.isKeyPressed(GLFW_KEY_LEFT))
            scene3d->camera->processMovement(s3d::Camera::LEFT, dt);
        if (window.isKeyPressed(GLFW_KEY_RIGHT))
            scene3d->camera->processMovement(s3d::Camera::RIGHT, dt);
        if (window.isKeyPressed(GLFW_KEY_UP))
            scene3d->camera->processMovement(s3d::Camera::FORWARD, dt);
        if (window.isKeyPressed(GLFW_KEY_DOWN))
            scene3d->camera->processMovement(s3d::Camera::BACKWARD, dt);

        glm::vec2 mouse = window.getMouseScreenPosition();
        float xpos = mouse.x;
        float ypos = mouse.y;
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        scene3d->camera->processMouseMovement(xoffset, yoffset);
    }

    void readData()
    {
        // Open the CSV file
        std::ifstream file("assets/data/data_clean.csv");

        // Check if the file is open
        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
        }

        // Define vectors to store each row of data
        std::vector<std::vector<std::string>> data;

        // Read the file line by line
        std::string line;
        while (std::getline(file, line)) {
            // Use a stringstream to split the line into tokens based on commas
            std::stringstream ss(line);
            std::vector<std::string> row;
            std::string token;

            while (std::getline(ss, token, ',')) {
                row.push_back(token);
            }

            // Add the row to the data vector
            data.push_back(row);
        }

        // Close the file
        file.close();

        // Display the imported data
        unsigned int idx = 0;
        std::vector<float> gravity = {};
        for (auto& row : data)
        {
            if (idx > 0)
                gravity.push_back(std::stof(row[5]));
            else
                idx++;
        }
        std::cout << "Heights: " << gravity.size() << std::endl;

        // add surface
        s3d::Surface* surface = new s3d::Surface(gravity, 178, 900, resourceManager.getShader("surface_shader"));
        scene3d->surfaces.push_back(surface);

    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
