#include "ScriptingSystem.h"
#include "Crocodile/utils/Logger.h"
#include "GLMBindings.h"
#include "KeyMouseBindings.h"

namespace Crocodile
{
	namespace scripting
	{
		ScriptingSystem::ScriptingSystem()
		{
			lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::string);
			registerLuaFunctions();
		}

		void ScriptingSystem::update()
		{
			if (!enabled)
				return;

			if (!m_mainScriptLoaded)
			{
				LOG(ERROR, "Main Lua script has not been loaded");
				return;
			}

			sol::table main_lua = lua["main"];
			sol::table update_script = main_lua[1];
			update_script["update"]();

			lua.collect_garbage();
		}

		void ScriptingSystem::registerLuaFunctions()
		{
			lua.set_function( "run_script", [ & ]( const std::string& path ) {
				try
				{
					lua.safe_script_file( path );
				}
				catch ( const sol::error& error )
				{
					LOG(ERROR, "Error loading Lua Script: " + std::string(error.what()));
				}
			});

			addGLMBindings(lua);
			addInputBindings(lua);
		}

		bool ScriptingSystem::loadMainScript()
		{
			try
			{
				lua.script_file(m_scriptPath);
			}
			catch (const sol::error& e)
			{
				LOG(ERROR, "Failed to load script: " + std::string(e.what()));
				return false;
			}

			sol::table main_lua = lua["main"];
			sol::optional<sol::table> updateExists = main_lua[1];
			if (updateExists == sol::nullopt)
			{
				LOG(ERROR, "No update function detected in main.lua");
				return false;
			}

			sol::table update_script = main_lua[1];
			sol::function _update = update_script["update"];

			m_mainScriptLoaded = true;
			LOG(INFO, "Main Lua script successfully loaded");
			return m_mainScriptLoaded;
		}

	}
}
