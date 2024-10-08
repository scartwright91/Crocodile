#pragma once

#include <sol/sol.hpp>
#include <GLFW/glfw3.h>

namespace Crocodile
{
    namespace scripting
    {
        static void addKeyBindings(sol::state &lua)
        {
            lua["KEY_UNKNOWN"] = GLFW_KEY_UNKNOWN;
            lua["KEY_SPACE"] = GLFW_KEY_SPACE;
            lua["KEY_APOSTROPHE"] = GLFW_KEY_APOSTROPHE;
            lua["KEY_COMMA"] = GLFW_KEY_COMMA;
            lua["KEY_MINUS"] = GLFW_KEY_MINUS;
            lua["KEY_PERIOD"] = GLFW_KEY_PERIOD;
            lua["KEY_SLASH"] = GLFW_KEY_SLASH;
            lua["KEY_0"] = GLFW_KEY_0;
            lua["KEY_1"] = GLFW_KEY_1;
            lua["KEY_2"] = GLFW_KEY_2;
            lua["KEY_3"] = GLFW_KEY_3;
            lua["KEY_4"] = GLFW_KEY_4;
            lua["KEY_5"] = GLFW_KEY_5;
            lua["KEY_6"] = GLFW_KEY_6;
            lua["KEY_7"] = GLFW_KEY_7;
            lua["KEY_8"] = GLFW_KEY_8;
            lua["KEY_9"] = GLFW_KEY_9;
            lua["KEY_SEMICOLON"] = GLFW_KEY_SEMICOLON;
            lua["KEY_EQUAL"] = GLFW_KEY_EQUAL;
            lua["KEY_A"] = GLFW_KEY_A;
            lua["KEY_B"] = GLFW_KEY_B;
            lua["KEY_C"] = GLFW_KEY_C;
            lua["KEY_D"] = GLFW_KEY_D;
            lua["KEY_E"] = GLFW_KEY_E;
            lua["KEY_F"] = GLFW_KEY_F;
            lua["KEY_G"] = GLFW_KEY_G;
            lua["KEY_H"] = GLFW_KEY_H;
            lua["KEY_I"] = GLFW_KEY_I;
            lua["KEY_J"] = GLFW_KEY_J;
            lua["KEY_K"] = GLFW_KEY_K;
            lua["KEY_L"] = GLFW_KEY_L;
            lua["KEY_M"] = GLFW_KEY_M;
            lua["KEY_N"] = GLFW_KEY_N;
            lua["KEY_O"] = GLFW_KEY_O;
            lua["KEY_P"] = GLFW_KEY_P;
            lua["KEY_Q"] = GLFW_KEY_Q;
            lua["KEY_R"] = GLFW_KEY_R;
            lua["KEY_S"] = GLFW_KEY_S;
            lua["KEY_T"] = GLFW_KEY_T;
            lua["KEY_U"] = GLFW_KEY_U;
            lua["KEY_V"] = GLFW_KEY_V;
            lua["KEY_W"] = GLFW_KEY_W;
            lua["KEY_X"] = GLFW_KEY_X;
            lua["KEY_Y"] = GLFW_KEY_Y;
            lua["KEY_Z"] = GLFW_KEY_Z;
            lua["KEY_LEFT_BRACKET"] = GLFW_KEY_LEFT_BRACKET;
            lua["KEY_BACKSLASH"] = GLFW_KEY_BACKSLASH;
            lua["KEY_RIGHT_BRACKET"] = GLFW_KEY_RIGHT_BRACKET;
            lua["KEY_GRAVE_ACCENT"] = GLFW_KEY_GRAVE_ACCENT;
            lua["KEY_WORLD_1"] = GLFW_KEY_WORLD_1;
            lua["KEY_WORLD_2"] = GLFW_KEY_WORLD_2;
            lua["KEY_ESCAPE"] = GLFW_KEY_ESCAPE;
            lua["KEY_ENTER"] = GLFW_KEY_ENTER;
            lua["KEY_TAB"] = GLFW_KEY_TAB;
            lua["KEY_BACKSPACE"] = GLFW_KEY_BACKSPACE;
            lua["KEY_INSERT"] = GLFW_KEY_INSERT;
            lua["KEY_DELETE"] = GLFW_KEY_DELETE;
            lua["KEY_RIGHT"] = GLFW_KEY_RIGHT;
            lua["KEY_LEFT"] = GLFW_KEY_LEFT;
            lua["KEY_DOWN"] = GLFW_KEY_DOWN;
            lua["KEY_UP"] = GLFW_KEY_UP;
            lua["KEY_PAGE_UP"] = GLFW_KEY_PAGE_UP;
            lua["KEY_PAGE_DOWN"] = GLFW_KEY_PAGE_DOWN;
            lua["KEY_HOME"] = GLFW_KEY_HOME;
            lua["KEY_END"] = GLFW_KEY_END;
            lua["KEY_CAPS_LOCK"] = GLFW_KEY_CAPS_LOCK;
            lua["KEY_SCROLL_LOCK"] = GLFW_KEY_SCROLL_LOCK;
            lua["KEY_NUM_LOCK"] = GLFW_KEY_NUM_LOCK;
            lua["KEY_PRINT_SCREEN"] = GLFW_KEY_PRINT_SCREEN;
            lua["KEY_PAUSE"] = GLFW_KEY_PAUSE;
            lua["KEY_F1"] = GLFW_KEY_F1;
            lua["KEY_F2"] = GLFW_KEY_F2;
            lua["KEY_F3"] = GLFW_KEY_F3;
            lua["KEY_F4"] = GLFW_KEY_F4;
            lua["KEY_F5"] = GLFW_KEY_F5;
            lua["KEY_F6"] = GLFW_KEY_F6;
            lua["KEY_F7"] = GLFW_KEY_F7;
            lua["KEY_F8"] = GLFW_KEY_F8;
            lua["KEY_F9"] = GLFW_KEY_F9;
            lua["KEY_F10"] = GLFW_KEY_F10;
            lua["KEY_F11"] = GLFW_KEY_F11;
            lua["KEY_F12"] = GLFW_KEY_F12;
            lua["KEY_F13"] = GLFW_KEY_F13;
            lua["KEY_F14"] = GLFW_KEY_F14;
            lua["KEY_F15"] = GLFW_KEY_F15;
            lua["KEY_F16"] = GLFW_KEY_F16;
            lua["KEY_F17"] = GLFW_KEY_F17;
            lua["KEY_F18"] = GLFW_KEY_F18;
            lua["KEY_F19"] = GLFW_KEY_F19;
            lua["KEY_F20"] = GLFW_KEY_F20;
            lua["KEY_F21"] = GLFW_KEY_F21;
            lua["KEY_F22"] = GLFW_KEY_F22;
            lua["KEY_F23"] = GLFW_KEY_F23;
            lua["KEY_F24"] = GLFW_KEY_F24;
            lua["KEY_F25"] = GLFW_KEY_F25;
            lua["KEY_KP_0"] = GLFW_KEY_KP_0;
            lua["KEY_KP_1"] = GLFW_KEY_KP_1;
            lua["KEY_KP_2"] = GLFW_KEY_KP_2;
            lua["KEY_KP_3"] = GLFW_KEY_KP_3;
            lua["KEY_KP_4"] = GLFW_KEY_KP_4;
            lua["KEY_KP_5"] = GLFW_KEY_KP_5;
            lua["KEY_KP_6"] = GLFW_KEY_KP_6;
            lua["KEY_KP_7"] = GLFW_KEY_KP_7;
            lua["KEY_KP_8"] = GLFW_KEY_KP_8;
            lua["KEY_KP_9"] = GLFW_KEY_KP_9;
            lua["KEY_KP_DECIMAL"] = GLFW_KEY_KP_DECIMAL;
            lua["KEY_KP_DIVIDE"] = GLFW_KEY_KP_DIVIDE;
            lua["KEY_KP_MULTIPLY"] = GLFW_KEY_KP_MULTIPLY;
            lua["KEY_KP_SUBTRACT"] = GLFW_KEY_KP_SUBTRACT;
            lua["KEY_KP_ADD"] = GLFW_KEY_KP_ADD;
            lua["KEY_KP_ENTER"] = GLFW_KEY_KP_ENTER;
            lua["KEY_KP_EQUAL"] = GLFW_KEY_KP_EQUAL;
            lua["KEY_LEFT_SHIFT"] = GLFW_KEY_LEFT_SHIFT;
            lua["KEY_LEFT_CONTROL"] = GLFW_KEY_LEFT_CONTROL;
            lua["KEY_LEFT_ALT"] = GLFW_KEY_LEFT_ALT;
            lua["KEY_LEFT_SUPER"] = GLFW_KEY_LEFT_SUPER;
            lua["KEY_RIGHT_SHIFT"] = GLFW_KEY_RIGHT_SHIFT;
            lua["KEY_RIGHT_CONTROL"] = GLFW_KEY_RIGHT_CONTROL;
            lua["KEY_RIGHT_ALT"] = GLFW_KEY_RIGHT_ALT;
            lua["KEY_RIGHT_SUPER"] = GLFW_KEY_RIGHT_SUPER;
            lua["KEY_MENU"] = GLFW_KEY_MENU;
        }

        static void addMouseBindings(sol::state &lua)
        {
            lua["MOUSE_LEFT"] = GLFW_MOUSE_BUTTON_1;
            lua["MOUSE_RIGHT"] = GLFW_MOUSE_BUTTON_2;
        }

        static void addJoystickBindings(sol::state &lua)
        {

        }

        static void addInputBindings(sol::state &lua)
        {
            addKeyBindings(lua);
            addMouseBindings(lua);
            addJoystickBindings(lua);
        }

    }
}
