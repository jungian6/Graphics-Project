#include "Vector.h"
#include <cmath>


Vector::Vector()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Vector::Vector(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;

}

Vector::~Vector()
{
}

float Vector::GetX() const
{
	return _x;
}

void Vector::SetX(const float x)
{
	_x = x;
}

float Vector::GetY() const
{
	return _y;
}

void Vector::SetY(const float y)
{
	_y = y;
}

float Vector::GetZ() const
{
	return _z;
}

void Vector::SetZ(const float z)
{
	_z = z;
}

float Vector::DotProduct(const Vector& v1, const Vector& v2)
{
	return v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY() + v1.GetZ() * v2.GetZ();
}

Vector Vector::CrossProduct(const Vector& v1, const Vector& v2)
{
	return Vector(
		v1.GetY() * v2.GetZ() - v1.GetZ() * v2.GetY(),
		v1.GetZ() * v2.GetX() - v1.GetX() * v2.GetZ(),
		v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX()
	);
}

Vector Vector::NormaliseVector(const Vector& vector)
{
	float magnitude = std::sqrt(vector.GetX() * vector.GetX() +
		vector.GetY() * vector.GetY() +
		vector.GetZ() * vector.GetZ());

	if (magnitude != 0)
	{
		return Vector(vector.GetX() / magnitude,
			vector.GetY() / magnitude,
			vector.GetZ() / magnitude);
	}
	else
	{
		// Return a zero vector if the magnitude is zero to avoid division by zero
		return Vector(0, 0, 0);
	}
}
