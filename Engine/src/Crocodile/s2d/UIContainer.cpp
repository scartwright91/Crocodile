#include "UIContainer.h"

namespace Crocodile
{
	namespace s2d
	{
		UIContainer::UIContainer() : padding(0.f)
		{
			m_renderMethod = "sprite";
			// default settings
			alignment = TOP;
			layout = VERTICAL;
			transition = NONE;
		}

		UIContainer::UIContainer(glm::vec2 padding) : padding(padding)
		{
			m_renderMethod = "sprite";
			// default settings
			alignment = TOP;
			layout = VERTICAL;
			transition = NONE;
			currentElementPosition = padding;
		}

		UIContainer::UIContainer(glm::vec2 padding, glm::vec2 size) : padding(padding)
		{
			m_renderMethod = "sprite";
			// default settings
			alignment = TOP;
			layout = VERTICAL;
			transition = NONE;
			setSize(size);
			currentElementPosition = padding;
		}

		UIContainer::~UIContainer()
		{
		}

		void UIContainer::addElementToUI(Object *obj)
		{
			obj->setPosition(getPosition());
			obj->move(currentElementPosition.x, currentElementPosition.y);
			glm::vec2 size = obj->getSize();
			updateCurrentElementPosition(size.x, size.y);
			if (!staticSize)
				updateContainer(size.x, size.y);
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
			glm::vec2 size = getSize();
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
