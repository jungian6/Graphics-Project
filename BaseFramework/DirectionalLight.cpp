#include "DirectionalLight.h"

DirectionalLighting::DirectionalLighting()
{
	_redLightValue = 0;
	_greenLightValue = 0;
	_blueLightValue = 0;
	_lightDirectionVector = Vector(0.0f, 0.0f, 0.0f);
}


DirectionalLighting::DirectionalLighting(const int& redLightValue, const int& greenLightValue, const int& blueLightValue, const Vector& lightDirectionVector)
{
	_redLightValue = redLightValue;
	_greenLightValue = greenLightValue;
	_blueLightValue = blueLightValue;
	_lightDirectionVector = lightDirectionVector;
}

DirectionalLighting::~DirectionalLighting()
{
}

int DirectionalLighting::GetRedValue() const
{
	return _redLightValue;
}

void DirectionalLighting::SetRedValue(const int redValue)
{
	_redLightValue = redValue;
}

int DirectionalLighting::GetGreenValue() const
{
	return _greenLightValue;
}

void DirectionalLighting::SetGreenValue(const int greenValue)
{
	_greenLightValue = greenValue;
}

int DirectionalLighting::GetBlueValue() const
{
	return _blueLightValue;
}

void DirectionalLighting::SetBlueValue(const int blueValue)
{
	_blueLightValue = blueValue;
}

Vector DirectionalLighting::GetLightDirectionVector() const
{
	return _lightDirectionVector;
}

void DirectionalLighting::SetLightDirectionVector(const Vector& lightDirectionVector)
{
	_lightDirectionVector = lightDirectionVector;
}

float DirectionalLighting::ClampRGBValues(const float& colourValue)
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
