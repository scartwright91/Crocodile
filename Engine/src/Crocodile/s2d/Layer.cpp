#include "Layer.h"

namespace Crocodile
{
	namespace s2d
	{

		Layer::Layer(std::string name)
		{
			m_name = name;
		}

		Layer::~Layer()
		{
		}

		void Layer::addObject(Object *obj)
		{
			m_objects.push_back(obj);
		}

		void Layer::removeObject(Object *obj)
		{
			for (unsigned int i = 0; i < m_objects.size(); i++)
			{
				if (m_objects[i]->m_id == obj->m_id)
				{
					m_objects.erase(m_objects.begin() + i);
					break;
				}
			}
		}

		void Layer::sort()
		{
			std::sort(m_objects.begin(), m_objects.end(), [](s2d::Object *o1, s2d::Object *o2)
					  { return o1->getYSortValue() < o2->getYSortValue(); });
		}

	}
}
