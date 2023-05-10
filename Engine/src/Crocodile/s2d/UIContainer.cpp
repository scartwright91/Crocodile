#include "UIContainer.h"

namespace Crocodile
{
	namespace s2d
	{
		UIContainer::UIContainer() : padding(0.f)
		{
			renderMethod = "sprite";
			// default settings
			alignment = TOP;
			layout = VERTICAL;
			transition = NONE;
		}

		UIContainer::UIContainer(glm::vec2 padding) : padding(padding)
		{
			renderMethod = "sprite";
			// default settings
			alignment = TOP;
			layout = VERTICAL;
			transition = NONE;
			currentElementPosition = padding;
		}

		UIContainer::UIContainer(glm::vec2 padding, glm::vec2 size) : padding(padding)
		{
			renderMethod = "sprite";
			// default settings
			alignment = TOP;
			layout = VERTICAL;
			transition = NONE;
			this->size = size;
			currentElementPosition = padding;
		}

		UIContainer::~UIContainer()
		{
		}

		void UIContainer::addElementToUI(Object *obj)
		{
			obj->setStartingPosition(getPosition());
			obj->move(currentElementPosition.x, currentElementPosition.y);
			updateCurrentElementPosition(obj->size.x, obj->size.y);
			if (!staticSize)
				updateContainer(obj->size.x, obj->size.y);
			addChild(obj);
		}

		void UIContainer::updateCurrentElementPosition(float width, float height)
		{
			if (layout == VERTICAL)
			{
				currentElementPosition.y += (height + padding.y);
			}
			else if (layout == HORIZONTAL)
			{
				currentElementPosition.x += (width + padding.x);
			}
		}

		void UIContainer::updateContainer(float width, float height)
		{
			if (nChildren == 0)
				size += padding;

			if (layout == VERTICAL)
			{
				size.y += (height + padding.y);
				if (width + padding.x > size.x)
					size.x = width + 2 * padding.x;
			}
			else if (layout == HORIZONTAL)
			{
				size.x += (width + padding.x);
				if (height + padding.y > size.y)
					size.y = height + 2 * padding.y;
			}
		}
	}
}
