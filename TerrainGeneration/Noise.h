#ifndef NOISE_H
#define NOISE_H

#include <cmath>

// made possible by to Auburn on github, thank you! //

struct NoiseValues;

class Noise final
{
public:
	explicit Noise(const NoiseValues& noiseValues);
	~Noise() = default;

	Noise(const Noise&) = delete;
	Noise(Noise&&) noexcept = delete;
	Noise& operator=(const Noise&) = delete;
	Noise& operator=(Noise&&) noexcept = delete;

	void SetSeed(int seed);
	int GetSeed() const;
	float GetNoise(float x, float y) const;

private:
	// static handy functions
	static float FastMin(float a, float b);
	static float FastMax(float a, float b);
	static float FastAbs(float f);
	static float FastSqrt(float f);
	static int FastFloor(float f);
	static int FastRound(float f);
	static float Lerp(float a, float b, float t);
	static float InterpHermite(float t);
	static float InterpQuintic(float t);
	static int Hash(int seed, int xPrimed, int yPrimed);

	struct Lookup
	{
		static const float Gradients2D[];
		static const float RandVecs2D[];
	};

	static const int PrimeX;
	static const int PrimeY;

	// default
	int m_Seed;
	const float m_ZoomValue; // Same as Frequency but Frequency is 1.f/ZoomValue

	// fractal
	const int m_Octaves;				// how many maps on each other
	const float m_Lacunarity;
	const float m_Gain;
	const float m_WeightedStrength;
	const float m_FractalBounding;

	float GenFractalFBm(float x, float y) const;
	float GenNoiseSingle(int seed, float x, float y) const;
	float SinglePerlin(int seed, float x, float y) const;
	float GradCoord(int seed, int xPrimed, int yPrimed, float xd, float yd) const;
};

#endif // !NOISE_H