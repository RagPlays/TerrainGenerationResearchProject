#include "Vector3.h"

#include <cassert>

#include "Vector2.h"

#include "MathHelpers.h"

const Vector3f Vector3f::UnitX = Vector3f{ 1.f, 0.f, 0.f };
const Vector3f Vector3f::UnitY = Vector3f{ 0.f, 1.f, 0.f };
const Vector3f Vector3f::UnitZ = Vector3f{ 0.f, 0.f, 1.f };
const Vector3f Vector3f::Zero = Vector3f{ 0.f, 0.f, 0.f };

Vector3f::Vector3f(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z)
{
}

Vector3f::Vector3f(const Vector3f& v)
	: x(v.x), y(v.y), z(v.z)
{
}

Vector3f::Vector3f(const Vector3f& from, const Vector3f& to)
	: x(to.x - from.x), y(to.y - from.y), z(to.z - from.z)
{
}

float Vector3f::Magnitude() const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector3f::SqrMagnitude() const
{
	return x * x + y * y + z * z;
}

float Vector3f::Normalize()
{
	const float m{ Magnitude() };
	if (m > 0)
	{
		const float invM{ 1.f / m };
		x *= invM;
		y *= invM;
		z *= invM;
	}

	return m;
}

Vector3f Vector3f::Normalized() const
{
	const float invM{ 1.f / Magnitude() };
	return { *this * invM };
}

float Vector3f::Dot(const Vector3f& v1, const Vector3f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3f Vector3f::Cross(const Vector3f& v1, const Vector3f& v2)
{
	return Vector3f
	{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

Vector3f Vector3f::Project(const Vector3f& v1, const Vector3f& v2)
{
	return { v2 * (Dot(v1, v2) / Dot(v2, v2)) };
}

Vector3f Vector3f::Reject(const Vector3f& v1, const Vector3f& v2)
{
	return { v1 - v2 * (Dot(v1, v2) / Dot(v2, v2)) };
}

Vector3f Vector3f::Reflect(const Vector3f& v1, const Vector3f& v2)
{
	return { v1 - (2.f * Vector3f::Dot(v1, v2) * v2) };
}

Vector2 Vector3f::GetXY() const
{
	return { x, y };
}

#pragma region Operator Overloads

Vector3f Vector3f::operator*(float scale) const
{
	return { x * scale, y * scale, z * scale };
}

Vector3f Vector3f::operator/(float scale) const
{
	const float invScale{ 1.f / scale };
	return { x * invScale, y * invScale, z * invScale };
}

Vector3f Vector3f::operator+(const Vector3f& v) const
{
	return { x + v.x, y + v.y, z + v.z };
}

Vector3f Vector3f::operator-(const Vector3f& v) const
{
	return { x - v.x, y - v.y, z - v.z };
}

Vector3f Vector3f::operator-() const
{
	return { -x ,-y,-z };
}

Vector3f& Vector3f::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

Vector3f& Vector3f::operator/=(float scale)
{
	const float invScale{ 1.f / scale };
	x *= invScale;
	y *= invScale;
	z *= invScale;
	return *this;
}

Vector3f& Vector3f::operator-=(const Vector3f& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3f& Vector3f::operator+=(const Vector3f& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

float& Vector3f::operator[](int index)
{
	switch (index)
	{
	case 0: return x;
	case 1: return y;
	case 2: return z;
	}

	// cant be right
	assert(false);
	return x;
}

float Vector3f::operator[](int index) const
{
	switch (index)
	{
	case 0: return x;
	case 1: return y;
	case 2: return z;
	}

	// cant be right
	assert(false);
	return x;
}

bool Vector3f::operator==(const Vector3f& v) const
{
	return AreEqual(x, v.x) && AreEqual(y, v.y) && AreEqual(z, v.z);
}