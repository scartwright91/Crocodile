#pragma once

#include <string>

#include "../Core.h"
#include <sol/sol.hpp>


namespace Crocodile
{
	namespace scripting
	{
		class CROCODILE_API ScriptingSystem
		{
		public:
			ScriptingSystem();

			void update();
			bool loadMainScript();
			void registerLuaFunctions();

			bool enabled = true;
			sol::state lua;

		private:
			std::string m_scriptPath = "res/scripts/main.lua";
			bool m_mainScriptLoaded = false;

		};
	}
}
