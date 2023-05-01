#pragma once

#include <iostream>

#include "../../graphics/Shader.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <map>

#include "../../Core.h"

#include "../../../utils/fonts/vollkorn_regular.h"
#include "../../../utils/fonts/habana.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Crocodile
{
	namespace s2d
	{

		struct CROCODILE_API Character
		{
			unsigned int TextureID; // ID handle of the glyph texture
			glm::ivec2 Size;		// Size of glyph
			glm::ivec2 Bearing;		// Offset from baseline to left/top of glyph
			unsigned int Advance;	// Horizontal offset to advance to next glyph
		};

		class CROCODILE_API TextRenderer
		{

		public:
			TextRenderer(std::string fontPath, graphics::Shader *shader);
			~TextRenderer();

			void render(
				std::string text,
				glm::mat4 model,
				glm::mat4 view,
				glm::mat4 projection,
				glm::vec3 color,
				float alpha,
				glm::vec2 textScale);

		private:
			FT_Library ft;
			std::string fontPath;
			graphics::Shader *shader;
			unsigned int VAO, VBO;
			std::map<char, Character> characters;

			bool init();
		};

	}
}
