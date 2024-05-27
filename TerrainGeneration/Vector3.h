#ifndef VECTOR3_H
#define VECTOR3_H

struct Vector2;

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f() = default;
	Vector3f(float _x, float _y, float _z);
	Vector3f(const Vector3f& from, const Vector3f& to);
	Vector3f(const Vector3f& v);

	float Magnitude() const;
	float SqrMagnitude() const;
	float Normalize();
	Vector3f Normalized() const;

	static float Dot(const Vector3f& v1, const Vector3f& v2);
	static Vector3f Cross(const Vector3f& v1, const Vector3f& v2);
	static Vector3f Project(const Vector3f& v1, const Vector3f& v2);
	static Vector3f Reject(const Vector3f& v1, const Vector3f& v2);
	static Vector3f Reflect(const Vector3f& v1, const Vector3f& v2);

	Vector2 GetXY() const;

	//Member Operators
	Vector3f operator*(float scale) const;
	Vector3f operator/(float scale) const;
	Vector3f operator+(const Vector3f& v) const;
	Vector3f operator-(const Vector3f& v) const;
	Vector3f operator-() const;
	Vector3f& operator+=(const Vector3f& v);
	Vector3f& operator-=(const Vector3f& v);
	Vector3f& operator/=(float scale);
	Vector3f& operator*=(float scale);
	float& operator[](const int index);
	float operator[](const int index) const;

	bool operator==(const Vector3f& v) const;

	static const Vector3f UnitX;
	static const Vector3f UnitY;
	static const Vector3f UnitZ;
	static const Vector3f Zero;
};

//Global Operators
inline Vector3f operator*(float scale, const Vector3f& v)
{
	return { v.x * scale, v.y * scale, v.z * scale };
}

#endif // !VECTOR3_H
