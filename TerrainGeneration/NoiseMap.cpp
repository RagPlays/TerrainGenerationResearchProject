#include "NoiseMap.h"

#include <iostream>
#include <cassert>

#include "Noise.h"

// constructor
NoiseMap::NoiseMap(int width, int height, const NoiseValues& noiseValues)
	: m_MapWidth{ width }
	, m_MapHeight{ height }
	, m_MapSize{ width * height }
	, m_NoiseConfig{ noiseValues }
{
	// resize
	m_NoiseMap.resize(m_MapSize); // -> unchangeable once created!

	// Load the noisemap
	LoadMap();
}

int NoiseMap::GetWidth() const
{
	return m_MapWidth;
}

int NoiseMap::GetHeight() const
{
	return m_MapHeight;
}

float NoiseMap::GetNoise(const Point2i& point)
{
	if (m_MapWidth < point.x || m_MapHeight < point.y) return -1;

	const int idx{ point.x + point.y * m_MapWidth };

	if (idx < 0 || m_MapSize <= idx) return -1.f;

	return (m_NoiseMap[idx] + 1.f) * 0.5f; // set from [-1, 1] to [0, 1] range
}

float NoiseMap::GetNoise(int x, int y) const
{
	if (m_MapWidth < x || m_MapHeight < y) return -1;

	const int idx{ x + y * m_MapWidth };

	if (idx < 0 || m_MapSize <= idx ) return -1.f;

	return (m_NoiseMap[idx] + 1.f) * 0.5f; // set from [-1, 1] to [0, 1] range
}

const std::vector<float>& NoiseMap::GetMap() const
{
	return m_NoiseMap;
}

const NoiseValues& NoiseMap::GetNoiseConfig() const
{
	return m_NoiseConfig;
}

void NoiseMap::SetSeed(int seed)
{
	// set seed
	m_NoiseConfig.seed = seed;

	// reload map
	LoadMap();
}

void NoiseMap::SetZoomValue(float zoomValue)
{
	// set zoom
	m_NoiseConfig.zoom = zoomValue;

	// reload map
	LoadMap();
}

void NoiseMap::SetNoiseConfig(const NoiseValues& noiseValues)
{
	// set noiseConfig
	m_NoiseConfig = noiseValues;

	// reload map
	LoadMap();
}

void NoiseMap::ChangeZoomValue(float zoomValueDifferents)
{
	// change zoomvalue
	m_NoiseConfig.zoom += zoomValueDifferents;

	// reload map
	LoadMap();
}

void NoiseMap::LoadMap()
{
	// Create Obj to get the noise from
	const Noise noise{ m_NoiseConfig };

	// go over the whole map
	for (int y{}, yWidth{}; y < m_MapHeight; ++y, yWidth += m_MapWidth)
	{
		for (int x{}; x < m_MapWidth; ++x)
		{
			const int idx{ x + yWidth };
			m_NoiseMap[idx] = noise.GetNoise(static_cast<float>(x), static_cast<float>(y));
		}
	}

	std::cout << "Map Loaded!\n";
}