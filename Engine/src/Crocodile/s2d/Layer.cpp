#include "Layer.h"


namespace Crocodile
{
	namespace s2d
	{

		Layer::Layer(std::string name)
		{
			this->name = name;
		}

		Layer::Layer(std::string name, float depth)
		{
			this->name = name;
			this->depth = depth;
		}

		Layer::~Layer()
		{

		}

		void Layer::addObject(Object* obj)
		{
			objects.push_back(obj);
		}

		void Layer::removeObject(Object* obj)
		{
			objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
		}

		void Layer::sort()
		{
			std::sort(objects.begin(), objects.end(), [](s2d::Object* o1, s2d::Object* o2) {return o1->getYSortValue() < o2->getYSortValue(); });
		}

	}
}
