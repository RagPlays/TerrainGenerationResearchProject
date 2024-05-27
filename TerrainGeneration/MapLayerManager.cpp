#include "MapLayerManager.h"
#include "Vector3.h"

MapLayerManager::MapLayerManager()
	: m_DefaultLayerColor{ ColorRGB{ 255, 102, 229 } }
{
}

MapLayerManager::~MapLayerManager()
{
	// delete all layers
	for (auto layer : m_pLayers)
	{
		delete layer;
	}
	m_pLayers.clear();
}

void MapLayerManager::AddLayer(const std::string& name, float height, const ColorRGB& color)
{
	m_pLayers.insert(new TerrainLayer{ name, height, color });
}

const ColorRGB& MapLayerManager::GetColorFromValue(float value) const
{
	// Go over all layers and search for right layer color
	for (const auto& layer : m_pLayers)
	{
		if (value < layer->height)
		{
			return layer->color;
		}
	}

	// if none is found return baseColor
	return m_DefaultLayerColor;
}

const ColorRGB& MapLayerManager::GetDefaultColor() const
{
	return m_DefaultLayerColor;
}
