#pragma once
#include "Framework.h"
#include "vertex.h"
#include <vector>
#include "matrix.h"
#include "MD2Loader.h"
#include "Model.h"
#include "Polygon3D.h"
#include "Camera.h"
#include "AmbientLighting.h"
#include "PointLighting.h"

class Rasteriser : public Framework
{
public:

	bool Initialise();
	void Update(const Bitmap& bitmap);
	void Render(const Bitmap& bitmap);
	void Shutdown();

	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateViewMatrix(float d, int width, int height);

	void DrawWireFrame(const Bitmap& bitmap);

	void DrawSolidFlat(const Bitmap& bitmap);

private:
	Model _model;
	Camera _camera;

	int _width;
	int _height;

	Matrix _modelTransformationMatrix;
	Matrix _perspectiveTransformationMatrix;
	Matrix _viewTransformationMatrix;

	Matrix _YrotationMatrix;
	std::vector<DirectionalLighting> _directionalLights;
	AmbientLighting _ambientLighting;
	std::vector<PointLighting> _pointLights;
};