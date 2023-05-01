#pragma once

#include "../Core.h"
#include "Object.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API UIContainer : public Object
		{

			enum UIAlign
			{
				TOP,
				LEFT,
				RIGHT,
				MIDDLE,
				BOTTOM
			};

			enum UILayout
			{
				HORIZONTAL,
				VERTICAL,
				STACK
			};

			enum UITransition
			{
				NONE,
				FADE,
				SHAKE,
				POP
			};

		public:

			UIAlign alignment;
			UILayout layout;
			UITransition transition;

			bool staticSize = false; // size can grow with new elements
			glm::vec2 padding;

			UIContainer();
			UIContainer(glm::vec2 padding);
			UIContainer(glm::vec2 padding, glm::vec2 size);
			~UIContainer();

			void addElementToUI(Object* obj);

			inline float getContainerWidth() { return containerWidth; }
			inline float getContainerHeight() { return containerHeight; }

		private:
			float containerWidth = 0.0f;
			float containerHeight = 0.0f;
			glm::vec2 currentElementPosition = glm::vec2(0.0f);
			void updateCurrentElementPosition(float width, float height);
			void updateContainer(float width, float height);

		};
	}
}

