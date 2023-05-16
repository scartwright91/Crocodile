#pragma once

#include <string>
#include <vector>

#include "../Core.h"
#include "Object.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API Layer
		{

		public:
			Layer(std::string name);
			Layer(std::string name, float depth);
			~Layer();

			void addObject(Object *obj);
			void removeObject(Object *obj);
			void sort();

			// layer characteristics
			std::string name = "";
			float depth = 0.f;
			bool ySort = false;
			bool cameraScroll = true;
			bool applyZoom = true;
			bool hide = false;

			std::vector<Object *> objects = {};
		};
	}
}
