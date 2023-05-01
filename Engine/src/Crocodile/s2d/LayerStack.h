#pragma once

#include "Layer.h"

#include "../Core.h"

#include <vector>

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API LayerStack
		{
		public:

			LayerStack();
			~LayerStack();

			void addLayer(Layer* layer);
			void insertLayer(Layer* layer, unsigned int position);
			void removeLayer(Layer* layer);
			void clearLayer(std::string name);
			void removeAllLayers();
			Layer* getLayer(std::string name);

			unsigned int inline getLayerNumber() { return layers.size(); }

			std::vector<Layer*> layers = {};

		};
	}
}

