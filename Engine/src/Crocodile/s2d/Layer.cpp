#include "Layer.h"

namespace Crocodile
{
	namespace s2d
	{

		Layer::Layer(std::string name)
		{
			this->name = name;
		}

		Layer::~Layer()
		{
		}

		void Layer::addObject(Object *obj)
		{
			objects.push_back(obj);
		}

		void Layer::removeObject(Object *obj)
		{
			for (unsigned int i = 0; i < objects.size(); i++)
			{
				if (objects[i]->id == obj->id)
				{
					objects.erase(objects.begin() + i);
					break;
				}
			}
		}

		void Layer::sort()
		{
			std::sort(objects.begin(), objects.end(), [](s2d::Object *o1, s2d::Object *o2)
					  { return o1->getYSortValue() < o2->getYSortValue(); });
		}

	}
}
