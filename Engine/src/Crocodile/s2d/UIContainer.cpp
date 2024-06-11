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
			m_size = size;
			currentElementPosition = padding;
		}

		UIContainer::~UIContainer()
		{
		}

		void UIContainer::addElementToUI(Object *obj)
		{
			obj->setPosition(getPosition());
			obj->move(currentElementPosition.x, currentElementPosition.y);
			updateCurrentElementPosition(obj->m_size.x, obj->m_size.y);
			if (!staticSize)
				updateContainer(obj->m_size.x, obj->m_size.y);
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
			if (layout == VERTICAL)
			{
				m_size.y += (height + padding.y);
				if (width + padding.x > m_size.x)
					m_size.x = width + 2 * padding.x;
			}
			else if (layout == HORIZONTAL)
			{
				m_size.x += (width + padding.x);
				if (height + padding.y > m_size.y)
					m_size.y = height + 2 * padding.y;
			}
		}
	}
}
