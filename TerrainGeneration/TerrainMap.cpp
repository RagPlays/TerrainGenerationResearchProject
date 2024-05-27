#include "TerrainMap.h"

#include <sstream>
#include <cassert>
#include <iostream>
#include <execution>

#include "SDL.h"
#include "SDL_surface.h"

#include "MapLayerManager.h"
#include "Vector3.h"

TerrainMap::TerrainMap(const Recti& terrainRect, const NoiseValues& noiseValues, const std::shared_ptr<MapLayerManager>& layerManager, SDL_Surface* backBuffer)
	: m_StartX{ terrainRect.left }
	, m_StartY{ terrainRect.top }
	, m_Width{ terrainRect.width }
	, m_Height{ terrainRect.height }
	, m_TerrainSize{ terrainRect.width * terrainRect.height }
	, m_pMapLayerManager{ layerManager }
	, m_pBackBuffer{ backBuffer }
	, m_SunHeight{ 2.f }
{
	assert(m_StartX > -1);
	assert(m_StartY > -1);
	assert((m_StartX + m_Width) <= backBuffer->w);
	assert((m_StartY + m_Height) <= backBuffer->h);

	m_pNoiseMap = std::make_unique<NoiseMap>(terrainRect.width, terrainRect.height, noiseValues);

	m_SunPos.z = m_SunHeight;
}

void TerrainMap::UpdateSunPos(int x, int y)
{
	m_SunPos.x = static_cast<float>(x);
	m_SunPos.y = static_cast<float>(y);
}

void TerrainMap::Render(RenderMode renderMode) const
{
	switch (renderMode)
	{
	case RenderMode::layermap:
		RenderLayerMap();
		break;

	case RenderMode::layerShadowMap:
		RenderLayerShadowMap();
		break;

	case RenderMode::noiseMap:
		RenderNoiseMap();
		break;

	default:
		std::cout << "SomeThing wrong in TERRAINMAP::RENDER\n";
		break;
	}
}

void TerrainMap::SetSeed(int seed) const
{
	m_pNoiseMap->SetSeed(seed);
}

void TerrainMap::ChangeZoom(float value)
{
	m_pNoiseMap->ChangeZoomValue(value);
}

void TerrainMap::RenderLayerMap() const
{
	ColorRGB pixelColor;

	for (int y{}; y < m_Height; ++y)
	{
		const int bufferY{ m_StartY + y };
		const int rowStart{ bufferY * m_pBackBuffer->w };

		for (int x{}; x < m_Width; ++x)
		{
			const int bufferX{ m_StartX + x };

			const float noiseValue{ m_pNoiseMap->GetNoise(x, y) };
			pixelColor = m_pMapLayerManager->GetColorFromValue(noiseValue);

			const int pixelNumber{ rowStart + bufferX };
			RenderPixel(pixelNumber, pixelColor);
		}
	}
}

void TerrainMap::RenderLayerShadowMap() const
{
	constexpr int SHADOWBRIGHTNESS{ 2 };
	constexpr float SHADOWBRIGHTNESSINV{ 1.f / static_cast<float>(SHADOWBRIGHTNESS) };
	ColorRGB pixelColor;

	for (int y{}; y < m_Height; ++y)
	{
		const int bufferY{ m_StartY + y };
		const int rowStart{ bufferY * m_pBackBuffer->w };

		for (int x{}; x < m_Width; ++x)
		{
			const int bufferX{ m_StartX + x };

			const float noiseValue{ m_pNoiseMap->GetNoise(x, y) };

			pixelColor = m_pMapLayerManager->GetColorFromValue(noiseValue);

			if (RayCast(x, y, noiseValue))
			{
				pixelColor.r = static_cast<int>(pixelColor.r * SHADOWBRIGHTNESSINV);
				pixelColor.g = static_cast<int>(pixelColor.g * SHADOWBRIGHTNESSINV);
				pixelColor.b = static_cast<int>(pixelColor.b * SHADOWBRIGHTNESSINV);
			}

			const int pixelNumber{ rowStart + bufferX };
			RenderPixel(pixelNumber, pixelColor);
		}
	}
}

void TerrainMap::RenderNoiseMap() const
{
	for (int y{}; y < m_Height; ++y)
	{
		const int bufferY{ m_StartY + y };
		const int rowStart{ bufferY * m_pBackBuffer->w };

		for (int x{}; x < m_Width; ++x)
		{
			const int bufferX{ m_StartX + x };

			const float noiseValue{ m_pNoiseMap->GetNoise(x, y) };

			const int pixelNumber{ rowStart + bufferX };
			RenderPixel(pixelNumber, ColorRGB{static_cast<int>(noiseValue * 255.f) } );
		}
	}
}

void TerrainMap::RenderPixel(int pixelNumber, const ColorRGB& color) const
{
	if (m_pBackBuffer == nullptr) return;

	uint32_t* const pixels{ static_cast<uint32_t*>(m_pBackBuffer->pixels) };

	const uint8_t r{ static_cast<uint8_t>(color.r) };
	const uint8_t g{ static_cast<uint8_t>(color.g) };
	const uint8_t b{ static_cast<uint8_t>(color.b) };

	pixels[pixelNumber] = SDL_MapRGB(m_pBackBuffer->format, r, g, b);
}

bool TerrainMap::RayCast(int mapX, int mapY, float height) const
{
	// raycast in here //
	constexpr int STEPS{ 75 };
	constexpr float MAXHEIGHT{ 1.f };

	// start point
	const Vector3f startPoint{ static_cast<float>(mapX), static_cast<float>(mapY), height }; // point on map

	// direction
	const Vector3f stepDirection{ (m_SunPos - startPoint) / static_cast<float>(STEPS) };

	// check
	for (int step{}; step < STEPS; ++step)
	{
		// calculate the step toward sun
		const Vector3f currentPos{ startPoint + (stepDirection * static_cast<float>(step)) };

		// noiseMap location
		const Point2i noisePoint{ static_cast<int>(currentPos.x), static_cast<int>(currentPos.y) };

		// get the height on that location
		const float noiseValue{ m_pNoiseMap->GetNoise(noisePoint) };

		// checks
		if (noiseValue < 0.f) return false;						// out of reachs
		if (currentPos.z > MAXHEIGHT) return false;				// to high
		if (noiseValue > currentPos.z) return true;				// in mountain
	}

	return false;
}