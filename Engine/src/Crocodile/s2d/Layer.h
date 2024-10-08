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

			std::vector<Object*> inline getObjects() const { return m_objects; };
			void addObject(Object *obj);
			void removeObject(Object *obj);
			void sort();
			void inline clear() { m_objects.clear(); };
			unsigned int inline getNumberOfObjects() { return (int)m_objects.size(); };

			// layer characteristics
			std::string m_name = "";
			// depth > 1.f: closer to camera, depth < 1.f: further from camera
			float m_depth = 1.f;
			// layer transparency
			float m_alpha = 1.f;
			// layer will sort all objects by increasing y-position at render time
			bool m_ySort = false;
			// layer will be moved by camera zoom / scrolling
			bool m_applyCamera = true;
			// layer visibility
			bool m_hide = false;

		private:
			std::vector<Object *> m_objects = {};
		};
	}
}
