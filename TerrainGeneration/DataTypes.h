#ifndef DATATYPES_H
#define DATATYPES_H

#include <vector>
#include <string>
#include "ColorRGB.h"



// Enum Classes ------

enum class RenderMode
{
	noiseMap = 0,
	layermap,
	layerShadowMap,
};

// -------------------



// Structs -----------

struct Point2i
{
	Point2i();
	explicit Point2i(int x, int y);

	int x;
	int y;
};

struct Point2f
{
	Point2f();
	explicit Point2f(float x, float y);

	float x;
	float y;
};

struct Point3f
{
	Point3f();
	explicit Point3f(float x, float y, float z);

	float x;
	float y;
	float z;
};


struct Recti
{
	Recti();
	explicit Recti(int left, int top, int width, int height);

	int left;
	int top;
	int width;
	int height;
};

struct NoiseValues
{
	NoiseValues();
	explicit NoiseValues(int seed, float zoom, int octaves, float lacunarity, float gain, float weightedStrength, float fractalBounding);

	int seed;
	float zoom;
	int octaves;
	float lacunarity;
	float gain;
	float weightedStrength;
	float fractalBounding;
};


struct TerrainLayer
{
	TerrainLayer();
	explicit TerrainLayer(const std::string& name, float height, const ColorRGB& color);
	std::string name;
	float height;
	ColorRGB color;
};

// -------------------

#endif // !DATATYPES_H