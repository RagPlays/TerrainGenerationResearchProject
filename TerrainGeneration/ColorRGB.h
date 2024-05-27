
#ifndef COLORRGB_H
#define COLORRGB_H

#include "MathHelpers.h"

struct ColorRGB
{
	ColorRGB()
		: r{}, g{}, b{}
	{
	}

	ColorRGB(int _r, int _g, int _b)
		: r{ _r }, g{ _g }, b{ _b }
	{
	}

	ColorRGB(int colorValue)
		: r{ colorValue }, g{ colorValue }, b{ colorValue }
	{
	}

	ColorRGB(const ColorRGB& other)
		: ColorRGB{ other.r, other.g, other.b }
	{
	}

	inline const ColorRGB& operator/=(int value)
	{
		r /= value;
		g /= value;
		b /= value;

		return *this;
	}

	int r;
	int g;
	int b;
};

#endif // !COLORRGB_H