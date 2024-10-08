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
				m_layers.push_back(layer);
			else
				LOG(WARNING, "Layer " + layer->m_name + " already exists in layer stack");
		}

		void LayerStack::insertLayer(Layer *layer, unsigned int position)
		{
			if (!layerExists(layer))
				m_layers.insert(m_layers.begin() + position, layer);
			else
				LOG(WARNING, "Layer " + layer->m_name + " already exists in layer stack");
		}

		void LayerStack::removeLayer(Layer *layer)
		{
			m_layers.erase(std::remove(m_layers.begin(), m_layers.end(), layer), m_layers.end());
		}

		void LayerStack::clearLayer(std::string name)
		{
			Layer *layer = getLayer(name);
			layer->clear();
		}

		Layer *LayerStack::getLayer(std::string name)
		{
			for (Layer *layer : m_layers)
				if (layer->m_name == name)
					return layer;
			LOG(ERROR, name + " not found in layer stack.");
			return NULL;
		}

		void LayerStack::moveLayerToPos(std::string name, unsigned int pos)
		{
			moveVectorElement(m_layers, getLayerPosition(name), pos);
		}

		void LayerStack::moveLayerUp(std::string name)
		{
			unsigned int currentPos = getLayerPosition(name);
			if (currentPos - 1 >= 0)
				moveVectorElement(m_layers, currentPos, currentPos - 1);
		}

		void LayerStack::moveLayerDown(std::string name)
		{
			unsigned int currentPos = getLayerPosition(name);
			if (currentPos + 1 < m_layers.size())
				moveVectorElement(m_layers, currentPos, currentPos + 1);
		}

		void LayerStack::removeAllLayers()
		{
			m_layers = {};
		}

		std::vector<std::string> LayerStack::getLayerNames()
		{
			std::vector<std::string> names = {};
			for (Layer *layer : m_layers)
				names.push_back(layer->m_name);
			return names;
		}

		unsigned int LayerStack::getLayerPosition(std::string name)
		{
			unsigned int pos = 0;
			for (int idx = 0; idx < m_layers.size(); idx++)
			{
				if (m_layers[idx]->m_name == name)
				{
					pos = idx;
					break;
				}
			}
			return pos;
		}

		unsigned int LayerStack::getNumberOfObjects()
		{
			unsigned int counter = 0;
			for (Layer *layer : m_layers)
				counter += layer->getNumberOfObjects();
			return counter;
		}

		bool LayerStack::layerExists(Layer *layer)
		{
			bool exists = false;
			for (Layer *l : m_layers)
				if (l->m_name == layer->m_name)
					exists = true;
			return exists;
		}
	}
}
