#include "Camera.h"

Camera::Camera(): Camera(0, 0, 0, Vertex(0, 0, 0, 1))
{
}

Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position)
{
	_xRotation = xRotation;
	_yRotation = yRotation;
	_zRotation = zRotation;
	_viewingPosition = position;

	_xTranslation = _viewingPosition.GetX();
	_yTranslation = _viewingPosition.GetY();
	_zTranslation = _viewingPosition.GetZ();

	Matrix xRotMatrix = { 1, 0, 0, 0,
					   0, cos(_xRotation), sin(_xRotation), 0,
					   0, -sin(_xRotation), cos(_xRotation), 0,
					   0, 0, 0, 1 };

	Matrix yRotMatrix = { cos(_yRotation), 0, -sin(_yRotation), 0,
				   0, 1, 0, 0,
				   sin(_yRotation), 0, cos(_yRotation), 0,
				   0, 0, 0, 1 };

	Matrix zRotMatrix = { cos(_zRotation), sin(_zRotation), 0, 0,
					   -sin(_zRotation), cos(_zRotation), 0, 0,
					   0, 0, 1, 0,
					   0, 0, 0, 1 };

	_viewingMatrix = Matrix{ 1, 0, 0, -_xTranslation,
							 0, 1, 0, -_yTranslation,
							 0, 0, 1, -_zTranslation,
							 0, 0, 0, 1 };

	_viewingMatrix = xRotMatrix * yRotMatrix * zRotMatrix * _viewingMatrix;
}

Camera::~Camera()
{
}

float Camera::GetX() const
{
	return _xRotation;
}

void Camera::SetX(const float xRotation)
{
	_xRotation = xRotation;
}

float Camera::GetY() const
{
	return _yRotation;
}

void Camera::SetY(const float yRotation)
{
	_yRotation = yRotation;
}

float Camera::GetZ() const
{
	return _zRotation;
}

void Camera::SetZ(const float zRotation)
{
	_zRotation = zRotation;
}

Matrix Camera::CreateViewingMatrix()
{
	return _viewingMatrix;
}

Vertex Camera::GetViewingPosition() const
{
	return _viewingPosition;
}
