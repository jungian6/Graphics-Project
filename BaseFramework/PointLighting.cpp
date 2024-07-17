#include "PointLighting.h"

PointLighting::PointLighting()
{
	_redLightValue = 0;
	_greenLightValue = 0;
	_blueLightValue = 0;
	_pointPosition = Vertex(0.0f, 0.0f, 0.0f, 1);
	_a = 0.0f;
	_b = 0.0f;
	_c = 0.0f;
}

PointLighting::PointLighting(const int& redLightValue, const int& greenLightValue, const int& blueLightValue, const Vertex& pointPosition, const float& a, const float& b, const float& c)
{
	_redLightValue = redLightValue;
	_greenLightValue = greenLightValue;
	_blueLightValue = blueLightValue;
	_pointPosition = pointPosition;
	_a = a;
	_b = b;
	_c = c;
}

PointLighting::~PointLighting()
{
}

int PointLighting::GetRedValue() const
{
	return _redLightValue;
}

void PointLighting::SetRedValue(const int redValue)
{
	_redLightValue = redValue;
}

int PointLighting::GetGreenValue() const
{
	return _greenLightValue;
}

void PointLighting::SetGreenValue(const int greenValue)
{
	_greenLightValue = greenValue;
}

int PointLighting::GetBlueValue() const
{
	return _blueLightValue;
}

void PointLighting::SetBlueValue(const int blueValue)
{
	_blueLightValue = blueValue;

}

float PointLighting::GetValueA() const
{
	return _a;
}

float PointLighting::GetValueB() const
{
	return _b;
}

float PointLighting::GetValueC() const
{
	return _c;
}

Vertex PointLighting::GetPointPosition() const
{
	return _pointPosition;
}

void PointLighting::SetPointPosition(const Vertex pointPosition)
{
	_pointPosition = pointPosition;
}

float PointLighting::ClampRGBValues(const float& colourValue)
{
	if (colourValue < 0) {
		return(int(0));
	}
	else if (colourValue > 255) {
		return(int(255));
	}
	else {
		return(colourValue);
	}
}
