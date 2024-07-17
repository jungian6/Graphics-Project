#include "Rasteriser.h"
#include <cmath>


Rasteriser app;

bool Rasteriser::Initialise()
{
	Vertex initiallisePosition = Vertex(0, 0, -50, 1);
	_camera = Camera(0.0f, 0.0f, 0.0f, initiallisePosition);

	_ambientLighting = AmbientLighting(60, 60, 60);

	_directionalLights.push_back(DirectionalLighting(255, 255, 255, Vector(10, 10, 1))); // White light from top-left-front
	//_directionalLights.push_back(DirectionalLighting(0, 0, 255, Vector(1, 0, -1)));       // Blue light from right

	//_pointLights.push_back(PointLighting(0, 255, 0, Vertex(-60, -90, -70, 1), 1.0f, 0.01f, 0.002f)); // Green light
	_pointLights.push_back(PointLighting(0, 0, 255, Vertex(0, 0, -50, 1), 1.0f, 0.05f, 0.002f)); // Blue light

	// Red light from top
	_directionalLights.push_back(DirectionalLighting(150, 0, 0, Vector(0, -10, 0)));


	if (!MD2Loader::LoadModel("C:\\MD2\\kenny.MD2", _model,
		&Model::AddPolygon,
		&Model::AddVertex))
	{
		return false;
	}

	// Set the model's material to be somewhat reflective
	// Set the model's color to red
	_model.SetAmbientReflection(1.0f, 0.8f, 0.8f);  // Red ambient reflection
	_model.SetDiffuseReflection(1.0f, 1.0f, 1.0f);  // Red diffuse reflection

	_modelTransformationMatrix = Matrix{ 1, 0, 0, 0,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1 };


	float num = 0.01f;
	float result = cos(num);
	float result2 = sin(num);
	float result3 = -sin(num);

	_YrotationMatrix = Matrix{ result, 0, result2, 0,
									 0, 1, 0, 0,
								   result3, 0, result, 0,
								   0, 0, 0, 1 };
	return true;
}

void Rasteriser::Update(const Bitmap& bitmap)
{
	_modelTransformationMatrix = _modelTransformationMatrix * _YrotationMatrix;

	_height = bitmap.GetHeight();
	_width = bitmap.GetWidth();
	float aspectRatio = _width / float(_height);
	float d = 1;
	GeneratePerspectiveMatrix(d, aspectRatio);
	GenerateViewMatrix(d, _width, _height);
}

void Rasteriser::Render(const Bitmap& bitmap)
{
	bitmap.Clear(RGB(0, 0, 0));
	_model.ApplyTransformToLocalVertices(_modelTransformationMatrix);
	_model.ApplyTransformToTransformedVertices(_camera.CreateViewingMatrix());
	_model.CalculateBackfaces(_camera);
	_model.Sort();
	_model.ApplyTransformToTransformedVertices(_perspectiveTransformationMatrix);
	_model.Dehomogensize();
	_model.ApplyTransformToTransformedVertices(_viewTransformationMatrix);

	// Lighting calculations
	_model.CalculateLightingAmbient(_ambientLighting);
	_model.CalculateLightingDirectional(_directionalLights);
	_model.CalculateLightingPoint(_pointLights);

	DrawSolidFlat(bitmap);



}

void Rasteriser::Shutdown()
{
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	_perspectiveTransformationMatrix = Matrix{ d / aspectRatio, 0, 0, 0,
									   0, d, 0, 0,
									   0, 0, d, 0,
									   0, 0, 1, 0 };
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	_viewTransformationMatrix = Matrix{ width / 2.0f, 0, 0, width / 2.0f,
									   0, -height / 2.0f, 0, height / 2.0f,
									   0, 0, d / 2.0f, d / 2.0f,
									   0, 0, 0, 1 };
}

void Rasteriser::DrawWireFrame(const Bitmap& bitmap)
{
	const vector<Polygon3D>& LocalPolygonCollection = _model.GetPolygons();
	const vector<Vertex>& LocalVertexCollection = _model.GetTransformedVertices();

	for (int i = 0; i < LocalPolygonCollection.size(); i++)
	{
		if (LocalPolygonCollection[i].GetCullState() == false)
		{
			// Get the indices of the vertices in the polygon
			int index0 = LocalPolygonCollection[i].GetIndex(0);
			int index1 = LocalPolygonCollection[i].GetIndex(1);
			int index2 = LocalPolygonCollection[i].GetIndex(2);

			// Get the vertices using the indices
			Vertex vertex0 = LocalVertexCollection[index0];
			Vertex vertex1 = LocalVertexCollection[index1];
			Vertex vertex2 = LocalVertexCollection[index2];

			// Create a vector of vertices
			vector<Vertex> PolygonVertices;

			// Add the vertices to the vector
			PolygonVertices.push_back(vertex0);
			PolygonVertices.push_back(vertex1);
			PolygonVertices.push_back(vertex2);

			HDC hdc = bitmap.GetDC();

			for (int k = 0; k <= 2; k++)
			{
				if (k == 0)
				{
					MoveToEx(hdc, (int)PolygonVertices[k].GetX(), (int)PolygonVertices[k].GetY(), NULL);
				}
				else
				{
					LineTo(hdc, (int)PolygonVertices[k].GetX(), (int)PolygonVertices[k].GetY());
				}
			}
			LineTo(hdc, (int)PolygonVertices[0].GetX(), (int)PolygonVertices[0].GetY());
		}
	};
}

void Rasteriser::DrawSolidFlat(const Bitmap& bitmap)
{
	// Get the local polygon list and local vertices collection
	const vector<Polygon3D>& localPolygonList = _model.GetPolygons();
	const vector<Vertex>& localVerticesCollection = _model.GetTransformedVertices();

	HDC hdc = bitmap.GetDC();

	for (const auto& polygon : localPolygonList)
	{
		if (!polygon.GetCullState())
		{
			HBRUSH brush = CreateSolidBrush(polygon.GetPolygon3dColor());
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

			POINT polygonPoints[3];

			for (int i = 0; i < 3; ++i)
			{
				// Gets the index of the vertex in the polygon
				int index = polygon.GetIndex(i);

				// Gets the vertex from the localVerticesCollection
				const Vertex& vertex = localVerticesCollection[index];

				// Gets the x and y coordinates of the vertex and stores them in the POINT structure
				polygonPoints[i].x = static_cast<LONG>(vertex.GetX());
				polygonPoints[i].y = static_cast<LONG>(vertex.GetY());
			}

			// Draws the polygon
			Polygon(hdc, polygonPoints, 3);

			SelectObject(hdc, oldBrush);
			DeleteObject(brush);
		}
	}
}