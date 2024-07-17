#pragma once

class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);

	~Vector();

	// Accessors
	float GetX() const;
	void SetX(const float x);

	float GetY() const;
	void SetY(const float y);

	float GetZ() const;
	void SetZ(const float z);

	// Static methods for DotProduct and CrossProduct
	static float DotProduct(const Vector& v1, const Vector& v2);
	static Vector CrossProduct(const Vector& v1, const Vector& v2);

	static Vector NormaliseVector(const Vector& vector);



private:
		float _x;
		float _y;
		float _z;
};

