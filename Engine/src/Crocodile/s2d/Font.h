#pragma once

#include "../../utils/fonts/vollkorn_regular.h"
#include "../../utils/fonts/habana.h"

#include <glm/glm.hpp>
#include <map>

#include "../Core.h"
#include "../graphics/Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Crocodile
{
    namespace s2d
    {

        class CROCODILE_API Font
        {
        public:
            Font()
            {
                init();
            }

            ~Font()
            {
            }

            glm::vec2 getTextSize(std::string text, glm::vec2 textScale)
            {
                std::string::const_iterator c;
                float x = 0.f;
                float y = 0.f;
                for (c = text.begin(); c != text.end(); c++)
                {
                    Character ch = characters[*c];
                    unsigned int adv = (ch.Advance >> 6);
                    int bearing = ch.Bearing.x == 0 ? 0 : 1;
                    x += adv + ch.Bearing.x;
                    float ySize = (2 * ch.Size.y - ch.Bearing.y) * textScale.y;
                    if (ySize > y)
                        y = ySize;
                    std::cout << *c << " bearing: " << ch.Bearing.x << " size: " << ch.Size.x << " advance: " << (ch.Advance >> 6) << std::endl;
                }

                return glm::vec2(x, y);
            }

        private:
            struct CROCODILE_API Character
            {
                unsigned int TextureID; // ID handle of the glyph texture
                glm::ivec2 Size;        // Size of glyph
                glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
                unsigned int Advance;   // Horizontal offset to advance to next glyph
            };

            FT_Library ft;
            unsigned int VAO, VBO;
            std::map<char, Character> characters;

            bool init()
            {
                // All functions return a value different than 0 whenever an error occurred
                if (FT_Init_FreeType(&ft))
                {
                    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
                    return false;
                }

                // load font as face
                FT_Face face;
                if (FT_New_Memory_Face(ft, Vollkorn_Regular_ttf, Vollkorn_Regular_ttf_len, 0, &face))
                // if (FT_New_Memory_Face(ft, Habana_Vieja_demo_ttf, Habana_Vieja_demo_ttf_len, 0, &face))
                {
                    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
                    return false;
                }
                else
                {
                    // set size to load glyphs as
                    FT_Set_Pixel_Sizes(face, 0, 48);

                    // disable byte-alignment restriction
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                    // load first 128 characters of ASCII set
                    for (unsigned char c = 0; c < 128; c++)
                    {
                        // Load character glyph
                        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                        {
                            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                            continue;
                        }
                        // generate texture
                        unsigned int texture;
                        glGenTextures(1, &texture);
                        glBindTexture(GL_TEXTURE_2D, texture);
                        glTexImage2D(
                            GL_TEXTURE_2D,
                            0,
                            GL_RED,
                            face->glyph->bitmap.width,
                            face->glyph->bitmap.rows,
                            0,
                            GL_RED,
                            GL_UNSIGNED_BYTE,
                            face->glyph->bitmap.buffer);
                        // set texture options
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        // now store character for later use
                        Character character = {
                            texture,
                            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                            static_cast<unsigned int>(face->glyph->advance.x)};
                        characters.insert(std::pair<char, Character>(c, character));
                    }
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
                // destroy FreeType once we're finished
                FT_Done_Face(face);
                FT_Done_FreeType(ft);

                return true;
            }
        };
    }
}