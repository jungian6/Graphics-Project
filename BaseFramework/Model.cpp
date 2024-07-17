#include "Model.h"
#include <algorithm>
#include <cmath>  // for fmin and fmax as an alternative

Model::Model()
{
	// Initialize reflection coefficients to 1.0f for testing
	_ka_red = _ka_green = _ka_blue = 1.0f;
	_kd_red = _kd_green = _kd_blue = 1.0f;
	_ks_red = _ks_green = _ks_blue = 1.0f;
}

Model::~Model()
{
}

const vector<Polygon3D>& Model::GetPolygons()
{
	return _polygons;
}

const vector<Vertex>& Model::GetVertices()
{
	return _vertices;
}

const vector<Vertex>& Model::GetTransformedVertices()
{
	return _transformedVertices;
}

size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	_vertices.push_back(Vertex(x, y, z, 1));
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	_polygons.push_back(Polygon3D(i0, i1, i2));
}

void Model::ApplyTransformToLocalVertices(const Matrix& transform)
{
	_transformedVertices.clear();
	for (int i = 0; i < _vertices.size(); i++)
	{
		_transformedVertices.push_back(transform * _vertices[i]);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix& transform)
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i] = transform * _transformedVertices[i];
	}
}

void Model::CalculateBackfaces(Camera _camera)
{
    for (size_t i = 0; i < _polygons.size(); i++)
    {
        // Get the 3 indices of the vertices that make up the polygon
        int index0 = _polygons[i].GetIndex(0);
        int index1 = _polygons[i].GetIndex(1);
        int index2 = _polygons[i].GetIndex(2);

        // Get the vertices for those indices
        const Vertex vertex0 = _transformedVertices[index0];
        const Vertex vertex1 = _transformedVertices[index1];
        const Vertex vertex2 = _transformedVertices[index2];

        // Construct vector a by subtracting vertex 1 from vertex 0
        Vector vectorA = vertex0 - vertex1;

        // Construct vector b by subtracting vertex 2 from vertex 0
        Vector vectorB = vertex0 - vertex2;

        // Calculate the normal vector from vector b and a
        Vector normal = Vector::CrossProduct(vectorB, vectorA);

        // Create eye-vector = vertex 0 - camera position
        Vector eyeVector = vertex0 - _camera.GetViewingPosition();

        // Take dot product of the normal and eye-vector
        float dotProduct = Vector::DotProduct(normal, eyeVector);

		if (dotProduct < 0.0f)
		{
			_polygons[i].SetCullState(true);
		}
		else
		{
			_polygons[i].SetCullState(false);
		}
        _polygons[i].SetNormal(normal);
    }
}

void Model::Dehomogensize()
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i].VertexDehomogenize();
	}
}

void Model::Sort(void)
{
    for (int i = 0; i < _polygons.size(); i++)
    {
        // Get the vertices for those indices
        const Vertex& vertex0 = _transformedVertices[_polygons[i].GetIndex(0)];
        const Vertex& vertex1 = _transformedVertices[_polygons[i].GetIndex(1)];
        const Vertex& vertex2 = _transformedVertices[_polygons[i].GetIndex(2)];

        // Calculate the average z value of the polygon
        float zAverage = (vertex0.GetZ() + vertex1.GetZ() + vertex2.GetZ()) / 3.0f;

        // Set the z value of the polygon
        _polygons[i].SetAverageZ(zAverage);
    }

    // Sort the polygons based on their z value (back to front)
    std::sort(_polygons.begin(), _polygons.end(), [](const Polygon3D& a, const Polygon3D& b) {
        return a.GetAverageZ() > b.GetAverageZ();  // Note the '>' instead of '<'
        });
}

//Binary Predicate for sorting by AVG Z
bool Model::sortByAvgZ(const Polygon3D& lhs, const Polygon3D& rhs)
{
	if (lhs.GetAverageZ() < rhs.GetAverageZ())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Model::CalculateLightingAmbient(const AmbientLighting& ambientLight)
{
    for (auto& polygon : _polygons)
    {
        if (!polygon.GetCullState())
        {
            float totalR = ambientLight.GetRedValue() * GetAmbientRed();
            float totalG = ambientLight.GetGreenValue() * GetAmbientGreen();
            float totalB = ambientLight.GetBlueValue() * GetAmbientBlue();

            // Use the same clamping method as in CalculateLightingDirectional
            totalR = DirectionalLighting::ClampRGBValues(totalR);
            totalG = DirectionalLighting::ClampRGBValues(totalG);
            totalB = DirectionalLighting::ClampRGBValues(totalB);

            polygon.SetPolygon3dColor(RGB(static_cast<int>(totalR),
                static_cast<int>(totalG),
                static_cast<int>(totalB)));
        }
    }
}

void Model::CalculateLightingPoint(const std::vector<PointLighting>& lights)
{
    for (auto& polygon : _polygons)
    {
        if (!polygon.GetCullState())
        {
            // Start with the existing color (set by ambient and directional lighting)
            COLORREF currentColor = polygon.GetPolygon3dColor();
            float totalRed = GetRValue(currentColor);
            float totalGreen = GetGValue(currentColor);
            float totalBlue = GetBValue(currentColor);

            // Calculate the center point of the polygon manually
            const Vertex& v1 = _transformedVertices[polygon.GetIndex(0)];
            const Vertex& v2 = _transformedVertices[polygon.GetIndex(1)];
            const Vertex& v3 = _transformedVertices[polygon.GetIndex(2)];

            Vertex centerPoint(
                (v1.GetX() + v2.GetX() + v3.GetX()) / 3.0f,
                (v1.GetY() + v2.GetY() + v3.GetY()) / 3.0f,
                (v1.GetZ() + v2.GetZ() + v3.GetZ()) / 3.0f,
                1.0f  // Assuming w-coordinate is 1 for a point
            );

            Vector normal = Vector::NormaliseVector(polygon.GetNormal());

            for (const auto& light : lights)
            {
                // Calculate vector to light source
                Vector lightDir(
                    light.GetPointPosition().GetX() - centerPoint.GetX(),
                    light.GetPointPosition().GetY() - centerPoint.GetY(),
                    light.GetPointPosition().GetZ() - centerPoint.GetZ()
                );

                // Calculate distance manually
                float distance = std::sqrt(
                    lightDir.GetX() * lightDir.GetX() +
                    lightDir.GetY() * lightDir.GetY() +
                    lightDir.GetZ() * lightDir.GetZ()
                );

                // Normalize light direction
                lightDir = Vector::NormaliseVector(lightDir);

                float dot = Vector::DotProduct(normal, lightDir);
                if (dot > 0)
                {
                    // Calculate attenuation
                    float attenuation = 1.0f / (light.GetValueA() + light.GetValueB() * distance + light.GetValueC() * distance * distance);

                    // Apply attenuation and intensity multiplier
                    float intensity = dot * attenuation * 100.0f;

                    totalRed += light.GetRedValue() * GetDiffuseRed() * intensity;
                    totalGreen += light.GetGreenValue() * GetDiffuseGreen() * intensity;
                    totalBlue += light.GetBlueValue() * GetDiffuseBlue() * intensity;
                }
            }

            // Clamp the color values
            totalRed = PointLighting::ClampRGBValues(totalRed);
            totalGreen = PointLighting::ClampRGBValues(totalGreen);
            totalBlue = PointLighting::ClampRGBValues(totalBlue);

            polygon.SetPolygon3dColor(RGB(static_cast<int>(totalRed),
                static_cast<int>(totalGreen),
                static_cast<int>(totalBlue)));
        }
    }
}



void Model::CalculateLightingDirectional(const std::vector<DirectionalLighting>& lights)
{
    float kd_red = GetDiffuseRed();
    float kd_green = GetDiffuseGreen();
    float kd_blue = GetDiffuseBlue();

    for (auto& polygon : _polygons)
    {
        if (!polygon.GetCullState())
        {
            COLORREF currentColor = polygon.GetPolygon3dColor();
            float totalRed = GetRValue(currentColor);
            float totalGreen = GetGValue(currentColor);
            float totalBlue = GetBValue(currentColor);

            Vector normal = Vector::NormaliseVector(polygon.GetNormal());

            for (const auto& light : lights)
            {
                Vector lightDir = Vector::NormaliseVector(light.GetLightDirectionVector());

                float dot = Vector::DotProduct(normal, lightDir);
                if (dot > 0)
                {
                    float scaleFactor = 0.5f; // Adjust this value to control the intensity of directional lights
                    totalRed += light.GetRedValue() * kd_red * dot * scaleFactor;
                    totalGreen += light.GetGreenValue() * kd_green * dot * scaleFactor;
                    totalBlue += light.GetBlueValue() * kd_blue * dot * scaleFactor;
                }
            }

            totalRed = DirectionalLighting::ClampRGBValues(totalRed);
            totalGreen = DirectionalLighting::ClampRGBValues(totalGreen);
            totalBlue = DirectionalLighting::ClampRGBValues(totalBlue);

            polygon.SetPolygon3dColor(RGB(static_cast<int>(totalRed),
                static_cast<int>(totalGreen),
                static_cast<int>(totalBlue)));
        }
    }
}


void Model::SetAmbientReflection(float r, float g, float b)
{
	_ka_red = r; _ka_green = g; _ka_blue = b;
}

void Model::SetDiffuseReflection(float r, float g, float b)
{
	_kd_red = r; _kd_green = g; _kd_blue = b;
}

void Model::SetSpecularReflection(float r, float g, float b)
{
	_ks_red = r; _ks_green = g; _ks_blue = b;
}

float Model::GetAmbientRed() const
{
	return _ka_red;
}

float Model::GetAmbientGreen() const
{
	return _ka_green;
}

float Model::GetAmbientBlue() const
{
	return _ka_blue;
}

float Model::GetDiffuseRed() const { return _kd_red; }
float Model::GetDiffuseGreen() const { return _kd_green; }
float Model::GetDiffuseBlue() const { return _kd_blue; }

float Model::GetSpecularRed() const { return _ks_red; }
float Model::GetSpecularGreen() const { return _ks_green; }
float Model::GetSpecularBlue() const { return _ks_blue; }
