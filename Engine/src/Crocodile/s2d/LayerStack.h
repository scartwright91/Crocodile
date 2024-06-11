#pragma once

#include "Layer.h"

#include "../Core.h"
#include "../../utils/misc.h"

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

			void addLayer(Layer *layer);
			void insertLayer(Layer *layer, unsigned int position);
			void removeLayer(Layer *layer);
			void clearLayer(std::string name);
			void moveLayerToPos(std::string name, unsigned int pos);
			void moveLayerUp(std::string name);
			void moveLayerDown(std::string name);
			void removeAllLayers();
			std::vector<std::string> getLayerNames();
			Layer *getLayer(std::string name);
			std::vector<Layer*> inline getLayers() const { return m_layers; };

			unsigned int getLayerPosition(std::string name);
			unsigned int inline getLayerNumber() { return (unsigned int)m_layers.size(); }

		private:
			std::vector<Layer *> m_layers = {};
			bool layerExists(Layer *layer);
		};
	}
}
