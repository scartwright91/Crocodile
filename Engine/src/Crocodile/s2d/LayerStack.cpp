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
			if (!layerExists(layer))
				layers.push_back(layer);
			else
				std::cout << "Layer " << layer->name << " already exists in layer stack" << std::endl;
		}

		void LayerStack::insertLayer(Layer *layer, unsigned int position)
		{
			if (!layerExists(layer))
				layers.insert(layers.begin() + position, layer);
			else
				std::cout << "Layer " << layer->name << " already exists in layer stack" << std::endl;
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

		void LayerStack::moveLayerToPos(std::string name, unsigned int pos)
		{
			moveVectorElement(layers, getLayerPosition(name), pos);
		}

		void LayerStack::moveLayerUp(std::string name)
		{
			unsigned int currentPos = getLayerPosition(name);
			if (currentPos - 1 >= 0)
				moveVectorElement(layers, currentPos, currentPos - 1);
		}

		void LayerStack::moveLayerDown(std::string name)
		{
			unsigned int currentPos = getLayerPosition(name);
			if (currentPos + 1 < layers.size())
				moveVectorElement(layers, currentPos, currentPos + 1);
		}

		void LayerStack::removeAllLayers()
		{
			layers = {};
		}

		std::vector<std::string> LayerStack::getLayerNames()
		{
			std::vector<std::string> names = {};
			for (Layer *layer : layers)
				names.push_back(layer->name);
			return names;
		}

		unsigned int LayerStack::getLayerPosition(std::string name)
		{
			unsigned int pos = 0;
			for (int idx = 0; idx < layers.size(); idx++)
			{
				if (layers[idx]->name == name)
				{
					pos = idx;
					break;
				}
			}
			return pos;
		}

		bool LayerStack::layerExists(Layer *layer)
		{
			bool exists = false;
			for (Layer *l : layers)
				if (l->name == layer->name)
					exists = true;
			return exists;
		}
	}
}
