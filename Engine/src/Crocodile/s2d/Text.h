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
            glm::vec2 textScale = glm::vec2(1.0f);

            Text()
            {
                this->renderType = TEXT;
            }

            Text(std::string text, bool unravelByLetter)
            {
                setText(text, unravelByLetter);
                this->renderType = TEXT;
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
                this->fullText = text;
                if (!unravelByLetter)
                    this->text = text;
                this->size = getTextSize();
            }

            bool isFullTextDisplayed()
            {
                return text == fullText;
            }


        private:

            Font font;
            bool unravelByLetter = false;

            // steps to be completed
            unsigned int step = 0;
            float counter = 0.0f;
            float textSpeed = 0.05f;
            unsigned int letterCounter = 0;

            glm::vec2 getTextSize()
            {
                return font.getTextSize(fullText, textScale);
            }
        };
	}
}

