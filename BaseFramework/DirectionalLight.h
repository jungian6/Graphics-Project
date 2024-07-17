#pragma once
#include "Vector.h"

class DirectionalLighting
{
public:

	/*
	Definition of the constructors needed to create an instance
	of directional lighting on the model
	*/

	DirectionalLighting();
	DirectionalLighting(const int& redLightValue, const int& greenLightValue, const int& blueLightValue, const Vector& lightDirectionVector);
	~DirectionalLighting();

	/*
	Accesses / mutates the light/direction vector values that the directional light has
	*/

	int GetRedValue() const;
	void SetRedValue(const int redValue);
	int GetGreenValue() const;
	void SetGreenValue(const int greenValue);
	int GetBlueValue() const;
	void SetBlueValue(const int blueValue);
	Vector GetLightDirectionVector() const;
	void SetLightDirectionVector(const Vector& lightDirectionVector);

	/*
	Clamps RGB values to between 0-255 so nothing is out of range after calculations
	*/

	static float ClampRGBValues(const float& colourValue);

private:

	/*
	Members to hold the light/vector values of directional lighting
	*/

	int _redLightValue;
	int _greenLightValue;
	int _blueLightValue;

	Vector _lightDirectionVector;

};