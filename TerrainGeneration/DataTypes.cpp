#include "DataTypes.h"


// ------------------------------------------------
//  Point2i
// ------------------------------------------------
Point2i::Point2i()
	: Point2i{ 0, 0 }
{
}

Point2i::Point2i(int x, int y)
	: x{ x }
	, y{ y }
{
}

// ------------------------------------------------
//  Point2f
// ------------------------------------------------
Point2f::Point2f()
	: Point2f{ 0.f, 0.f }
{
}

Point2f::Point2f(float x, float y)
	: x{ x }
	, y{ y }
{
}


// ------------------------------------------------
//  Point2f
// ------------------------------------------------
Point3f::Point3f()
	: Point3f{ 0.f, 0.f, 0.f }
{
}

Point3f::Point3f(float x, float y, float z)
	: x{ x }
	, y{ y }
	, z{ z }
{
}

// ------------------------------------------------
//  Recti
// ------------------------------------------------
Recti::Recti()
	: Recti{0, 0, 0, 0}
{
}

Recti::Recti(int left, int top, int width, int height)
	: left{ left }
	, top{ top }
	, width{ width }
	, height{ height }
{
}

// ------------------------------------------------
//  NoiseValues
// ------------------------------------------------
NoiseValues::NoiseValues()
	: NoiseValues{ 1337, 100.f, 10, 2.f, 0.5f, 0.f, 0.5f }
{
}

NoiseValues::NoiseValues(int seed, float zoom, int octaves, float lacunarity, float gain, float weightedStrength, float fractalBounding)
	: seed{ seed }
	, zoom{ zoom }
	, octaves{ octaves }
	, lacunarity{ lacunarity }
	, gain{ gain }
	, weightedStrength{ weightedStrength }
	, fractalBounding{ fractalBounding }
{
}

// ------------------------------------------------
//  TerrainLayer
// ------------------------------------------------
TerrainLayer::TerrainLayer()
	: TerrainLayer{ "NONE", -1.f, ColorRGB{} }
{
}

TerrainLayer::TerrainLayer(const std::string& name, float height, const ColorRGB& color)
	: name{ name }
	, height{ height }
	, color{ color }
{
}