#ifndef TERRAINMAP_H
#define TERRAINMAP_H

#include <memory>
#include "DataTypes.h"
#include "NoiseMap.h"

#include "Vector3.h"

#include <set>

class MapLayerManager;

struct SDL_Surface;
class NoiseMap;

struct Vector3f;

class TerrainMap final
{
public:
	explicit TerrainMap(const Recti& terrainRect, const NoiseValues& noiseValues, const std::shared_ptr<MapLayerManager>& layerManager, SDL_Surface* backBuffer);
	~TerrainMap() = default;

	TerrainMap(const TerrainMap&) = delete;
	TerrainMap(TerrainMap&&) noexcept = delete;
	TerrainMap& operator=(const TerrainMap&) = delete;
	TerrainMap& operator=(TerrainMap&&) noexcept = delete;

	void UpdateSunPos(int x, int y);
	void Render(RenderMode renderMode) const;
	void SetSeed(int seed) const;

	void ChangeZoom(float value);

private:
	
	// position and dimentions
	const int m_StartX;
	const int m_StartY;
	const int m_Width;
	const int m_Height;
	const int m_TerrainSize;

	// mouse
	Vector3f m_SunPos;
	const float m_SunHeight;
	
	// maps
	std::unique_ptr<NoiseMap> m_pNoiseMap;
	std::shared_ptr<MapLayerManager> m_pMapLayerManager;

	SDL_Surface* m_pBackBuffer; // this class does not own this

	// functions
	void RenderLayerMap() const;
	void RenderLayerShadowMap() const;
	void RenderNoiseMap() const;
	void RenderPixel(int pixelNumber, const ColorRGB& color) const;

	bool RayCast(int bufferX, int bufferY, float height) const;
};

#endif // !TERRAINMAP_H