#ifndef NOISEMAP_H
#define NOISEMAP_H

#include "DataTypes.h"

class NoiseMap final
{
public:
	explicit NoiseMap(int width, int height, const NoiseValues& noiseValues);
	~NoiseMap() = default;

	NoiseMap(const NoiseMap&) = delete;
	NoiseMap(NoiseMap&&) noexcept = delete;
	NoiseMap& operator=(const NoiseMap&) = delete;
	NoiseMap& operator=(NoiseMap&&) noexcept = delete;

	// getters / setters //

	int GetWidth() const;
	int GetHeight() const;
	// no out of reach checks!!
	float GetNoise(const Point2i& point);
	float GetNoise(int x, int y) const;
	const std::vector< float >& GetMap() const;
	const NoiseValues& GetNoiseConfig() const;

	void SetSeed(int seed);
	void SetZoomValue(float zoomValue);
	void SetNoiseConfig(const NoiseValues& noiseValues);

	void ChangeZoomValue(float zoomValueDifferents);

private:
	const int m_MapWidth;
	const int m_MapHeight;
	NoiseValues m_NoiseConfig;

	int m_MapSize;
	std::vector< float > m_NoiseMap;

	void LoadMap();
};

#endif // !NOISEMAP_H