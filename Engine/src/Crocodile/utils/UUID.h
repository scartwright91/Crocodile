#pragma once

namespace Crocodile
{
	class UUID
	{
	public:
		UUID();
		UUID(unsigned int uuid);

	private:
		unsigned int uuid;
	};
}