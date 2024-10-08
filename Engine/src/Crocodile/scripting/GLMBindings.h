#pragma once

#include <sol/sol.hpp>
#include <glm/glm.hpp>

namespace Crocodile
{
    namespace scripting
    {

        static void addVec2Bindings(sol::state &lua)
        {
            lua.new_usertype<glm::vec2>(
                "vec2",
                sol::call_constructor,
                sol::constructors<glm::vec2( float ), glm::vec2( float, float )>(),
                "x", &glm::vec2::x,
                "y", &glm::vec2::y
            );
        }
        
        static void addVec3Bindings(sol::state &lua)
        {
            lua.new_usertype<glm::vec3>(
                "vec3",
                sol::call_constructor,
                sol::constructors<glm::vec3( float ), glm::vec3( float, float, float )>(),
                "x", &glm::vec3::x,
                "y", &glm::vec3::y,
                "z", &glm::vec3::z
            );
        }

        static void addGLMBindings(sol::state &lua)
        {
            addVec2Bindings(lua);
            addVec3Bindings(lua);
        }
    }
}
