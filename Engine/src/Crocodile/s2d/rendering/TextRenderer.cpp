#include "TextRenderer.h"

namespace Crocodile
{
    namespace s2d
    {
        TextRenderer::TextRenderer(
            std::string fontPath,
            unsigned int fontSize,
            graphics::Shader *shader
        ) : fontPath(fontPath), fontSize(fontSize), shader(shader)
        {
            this->shader->setTexture("u_Text");
            init();
        }

        TextRenderer::~TextRenderer()
        {
            glDeleteVertexArrays(1, &this->VAO);
        }

        void TextRenderer::render(
            std::string text,
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 color,
            float alpha,
            glm::vec2 textScale,
            float layerAlpha)
        {

            // activate corresponding render state
            shader->use();
            // vertex
            shader->setMat4("u_Model", model);
            shader->setMat4("u_View", view);
            shader->setMat4("u_Projection", projection);
            // fragment
            shader->setVec3("u_TextColor", color);
            shader->setFloat("u_Alpha", alpha);
            shader->setFloat("u_LayerAlpha", layerAlpha);

            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(VAO);

            glm::vec3 position = model[3];
            float x = position.x;
            float y = position.y;

            // iterate through all characters
            std::string::const_iterator c;
            for (c = text.begin(); c != text.end(); c++)
            {
                Character ch = characters[*c];

                float xpos = x + ch.Bearing.x * textScale.x;
                float ypos = y + (characters['H'].Bearing.y - ch.Bearing.y) * textScale.y;

                float w = ch.Size.x * textScale.x;
                float h = ch.Size.y * textScale.y;
                // update VBO for each character
                float vertices[6][4] = {
                    {xpos, ypos + h, 0.0f, 1.0f},
                    {xpos + w, ypos, 1.0f, 0.0f},
                    {xpos, ypos, 0.0f, 0.0f},

                    {xpos, ypos + h, 0.0f, 1.0f},
                    {xpos + w, ypos + h, 1.0f, 1.0f},
                    {xpos + w, ypos, 1.0f, 0.0f}};
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.Advance >> 6) * textScale.x; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        bool TextRenderer::init()
        {
            // All functions return a value different than 0 whenever an error occurred
            if (FT_Init_FreeType(&ft))
            {
                std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
                return false;
            }

            // find path to font
            if (fontPath.empty())
            {
                std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
                return false;
            }

            // load font as face
            FT_Face face;
            if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
            {
                std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
                return false;
            }
            else
            {
                // set size to load glyphs as
                FT_Set_Pixel_Sizes(face, 0, fontSize);

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
#ifdef CROCODILE_EMSCRIPTEN
                    glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_LUMINANCE,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_LUMINANCE,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer);
#else
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
#endif
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

            // creating buffers
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            return true;
        }
    }
}