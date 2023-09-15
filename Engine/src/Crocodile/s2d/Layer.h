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
			~Layer();

			void addObject(Object *obj);
			void removeObject(Object *obj);
			void sort();

			// layer characteristics
			std::string name = "";
			// depth > 1.f: closer to camera, depth < 1.f: further from camera
			float depth = 1.f;
			// layer transparency
			float alpha = 1.f;
			// layer will sort all objects by increasing y-position at render time
			bool ySort = false;
			bool cameraScroll = true;
			bool applyZoom = true;
			bool hide = false;

			std::vector<Object *> objects = {};
		};
	}
}
