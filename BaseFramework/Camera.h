#pragma once
#include "vertex.h"
#include "matrix.h"
#include <cmath>

class Camera
{
public:

	Camera();
	Camera(float xRotation, float yRotation, float zRotation, const Vertex& position);
	~Camera();

	// Accessors
	float GetX() const;
	void SetX(const float xRotation);
	float GetY() const;
	void SetY(const float yRotation);
	float GetZ() const;
	void SetZ(const float zRotation);

	Matrix CreateViewingMatrix();

	Vertex GetViewingPosition() const;

private:
	float _xRotation;
	float _yRotation;
	float _zRotation;

	float _xTranslation;
	float _yTranslation;
	float _zTranslation;

	Matrix _viewingMatrix;

	Vertex _viewingPosition;
};

