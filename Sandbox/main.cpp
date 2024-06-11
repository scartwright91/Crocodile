
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "lua.hpp"

#include "Crocodile.h"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    s2d::Text* fps = new s2d::Text();
    s2d::Object* player = new s2d::Object();
    s2d::Object* box = new s2d::Object();

    Sandbox() : Crocodile::Application("Sandbox", true, 1280, 720, false)
    {
        init();
        runLua();
    }

    ~Sandbox()
    {
        delete fps;
        delete player;
    }

    void update(float dt)
    {
        fps->text = std::to_string(clock.getFPS());
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
            running = false;

    }

    void runLua()
    {
        // Create a new Lua state
        lua_State *L = luaL_newstate();

        // Load the Lua libraries
        luaL_openlibs(L);

        // Run the Lua script
        if (luaL_dofile(L, "res/scripts/player.lua")) {
            std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);  // Remove error message from the stack
        }

        // Get the 'add' function from Lua
        lua_getglobal(L, "add");
        if (!lua_isfunction(L, -1)) {
            std::cerr << "Error: 'add' is not a function" << std::endl;
            lua_pop(L, 1);  // Remove non-function value from the stack
        }

        lua_pushnumber(L, 10);
        lua_pushnumber(L, 20);

        // Call the function with 2 arguments and 1 result
        if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
            std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);  // Remove error message from the stack
        }

        // Get the result
        if (!lua_isnumber(L, -1)) {
            std::cerr << "Error: result is not a number" << std::endl;
            lua_pop(L, 1);  // Remove non-number result from the stack
        }

        double result = lua_tonumber(L, -1);
        std::cout << "Result: " << result << std::endl;

        // Close the Lua state
        lua_close(L);
    }

    void init()
    {
        resourceManager.addDirWatcher("res/shaders", SHADER);

        // create layers
        std::vector<std::string> layerNames = {"entities", "hud"};
        for (const std::string &layerName : layerNames)
        {
            s2d::Layer* layer = new s2d::Layer(layerName);
            if (layerName == "hud")
                layer->m_applyCamera = false;
            scene2d->m_layerStack->addLayer(layer);
        }

        // create player
        player->setPosition({500.f, 500.f});
        player->m_size = {50.f, 50.f};
        player->m_color = {1.f, 0.f, 0.f};

        // create box
        box->setPosition({400.f, 400.f});
        box->m_size = {100.f, 50.f};
        box->m_color = {0.f, 0.f, 1.f};

        scene2d->addObject(player, "entities");
        scene2d->addObject(box, "entities");
        scene2d->m_camera->setTarget(player, false);
        
    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
