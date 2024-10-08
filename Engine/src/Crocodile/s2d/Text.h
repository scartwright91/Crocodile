#pragma once

#include "../Core.h"
#include "Object.h"
#include "rendering/TextRenderer.h"
#include "Font.h"

namespace Crocodile
{
    namespace s2d
    {

        class CROCODILE_API Text : public Object
        {

        public:
            std::string fullText = "";
            std::string text = "";

            std::string fontName;

            Text() : fontName("default")
            {
                m_renderMethod = "text";
            }

            Text(std::string fontName) : fontName(fontName)
            {
                m_renderMethod = "text";
            }

            ~Text()
            {
            }

            void update(float dt)
            {
                if (unravelByLetter)
                {
                    if (letterCounter < fullText.size())
                    {
                        counter += dt;
                        if (counter >= textSpeed)
                        {
                            text += fullText[letterCounter];
                            counter = 0.0f;
                            letterCounter++;
                        }
                    }
                }
            }

            void setText(std::string text, bool unravelByLetter)
            {
                this->unravelByLetter = unravelByLetter;
                letterCounter = 0;
                this->fullText = text;
                if (!unravelByLetter)
                    this->text = text;
                else
                    this->text = "";
                setSize(font.getTextSize(fullText, textScale));
            }

            void setScale(glm::vec2 scale)
            {
                this->textScale = scale;
                setSize(font.getTextSize(fullText, textScale));
            }

            glm::vec2 getScale()
            {
                return textScale;
            }

            bool isFullTextDisplayed()
            {
                return text == fullText;
            }

        private:
            Font font;
            bool unravelByLetter = false;

            unsigned int step = 0;
            float counter = 0.0f;
            float textSpeed = 0.05f;
            unsigned int letterCounter = 0;
            glm::vec2 textScale = glm::vec2(1.0f);
        };
    }
}
