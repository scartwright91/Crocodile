#include "Interactive.h"

namespace Crocodile
{
	namespace s2d
	{

		Interactive::Interactive(Object* obj)
		{
			obj->addChild(this);
		}

		Interactive::~Interactive()
		{

		}

		void Interactive::onHover()
		{
			if (!hovered)
				hovered = true;
		}

		void Interactive::onClick()
		{
			if (!selected)
				selected = true;
		}

	}
}