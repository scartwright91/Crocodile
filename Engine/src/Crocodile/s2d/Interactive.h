#pragma once

#include "../Core.h"
#include "Object.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API Interactive : public Object
		{

		public:

			Interactive(Object* obj);
			~Interactive();

			void update();
			void onClick();
			void onHover();

		private:

			bool hovered = false;
			bool selected = false;

		};
	}
}

