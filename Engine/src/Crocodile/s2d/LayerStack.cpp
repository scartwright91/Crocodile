#include "LayerStack.h"

namespace Crocodile
{
	namespace s2d
	{

		LayerStack::LayerStack()
		{
		}

		LayerStack::~LayerStack()
		{
		}

		void LayerStack::addLayer(Layer *layer)
		{
			layers.push_back(layer);
		}

		void LayerStack::insertLayer(Layer *layer, unsigned int position)
		{
			layers.insert(layers.begin() + position, layer);
		}

		void LayerStack::removeLayer(Layer *layer)
		{
			layers.erase(std::remove(layers.begin(), layers.end(), layer), layers.end());
		}

		void LayerStack::clearLayer(std::string name)
		{
			Layer *layer = getLayer(name);
			layer->objects.clear();
		}

		Layer *LayerStack::getLayer(std::string name)
		{
			for (Layer *layer : layers)
				if (layer->name == name)
					return layer;
			std::cout << "ERROR: " << name << " not found in layer stack." << std::endl;
			return NULL;
		}

		void LayerStack::removeAllLayers()
		{
			layers = {};
		}

	}
}
