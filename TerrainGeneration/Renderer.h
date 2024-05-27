#ifndef RENDERER_H
#define RENDERER_H

#include "DataTypes.h"

struct SDL_Window;
struct SDL_Surface;

class Timer;
class NoiseMap;
class TerrainMap;
class MapLayerManager;

class Renderer final
{
public:
	explicit Renderer(SDL_Window* pWindow, int width, int height);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) noexcept = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) noexcept = delete;

	void Update(float elapsedSec);
	void Render() const;

	void ScrollWheelInput(float value);
	void NewMaps();
	void ToggleRenderModes();
	void ShowOnlyOneToggle();
	void ChangeMapSeed();
	void AddLayer();

	bool SaveBufferToImage() const;

private:
	SDL_Window* m_pWindow;

	SDL_Surface* m_pFrontBuffer;

	SDL_Surface* m_pBackBuffer;

	const int m_Width;
	const int m_Height;
	const int m_NrOfPixels;

	bool m_ShowOnlyOne;

	std::vector< TerrainMap* > m_pTerrainMaps;

	std::shared_ptr<MapLayerManager> m_pMapLayerManager;

	RenderMode m_RenderMode{ RenderMode::noiseMap };

	// private member functions
	void InitMaps();
	void InitMapLayers();

	void DestroyMaps();
};

#endif // !RENDERER_H