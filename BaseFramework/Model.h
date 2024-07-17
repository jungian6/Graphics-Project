#pragma once
#include "Polygon3D.h"
#include "Vertex.h"
#include <vector>
#include "Matrix.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "AmbientLighting.h"
#include "Pointlighting.h"

using namespace std;

class Model
{
public:
	Model();
	~Model();

	// Accessors
	const vector<Polygon3D>&	GetPolygons();
	const vector<Vertex>&		GetVertices();
	const vector< Vertex>&		GetTransformedVertices();
	size_t						GetPolygonCount() const;
	size_t						GetVertexCount() const;
	void						AddVertex(float x, float y, float z);
	void						AddPolygon(int i0, int i1, int i2);

	void ApplyTransformToLocalVertices(const Matrix &transform);
	void ApplyTransformToTransformedVertices(const Matrix &transform);

	void CalculateBackfaces(Camera _camera);

	void Dehomogensize();

	void Sort(void);

	static bool sortByAvgZ(const Polygon3D& lhs, const Polygon3D& rhs);

	void CalculateLightingAmbient(const AmbientLighting& ambientLight);

	void CalculateLightingPoint(const std::vector<PointLighting>& lights);


	// New methods for reflection coefficients
	void SetAmbientReflection(float r, float g, float b);
	void SetDiffuseReflection(float r, float g, float b);
	void SetSpecularReflection(float r, float g, float b);

	float GetAmbientRed() const;
	float GetAmbientGreen() const;
	float GetAmbientBlue() const;

	float GetDiffuseRed() const;
	float GetDiffuseGreen() const;
	float GetDiffuseBlue() const;

	float GetSpecularRed() const;
	float GetSpecularGreen() const;
	float GetSpecularBlue() const;

	void CalculateLightingDirectional(const std::vector<DirectionalLighting>& lightingVectors);



private:
	vector<Polygon3D>	_polygons;
	vector<Vertex>		_vertices;

	vector<Vertex>		_transformedVertices;

	Matrix transformationMatrix = Matrix{ 1, 0, 0, 0,
										  0, 1, 0, 0,
										  0, 0, 1, 0,
										  0, 0, 0, 1 };

	float _ka_red, _ka_green, _ka_blue;  // Ambient reflection
	float _kd_red, _kd_green, _kd_blue;  // Diffuse reflection
	float _ks_red, _ks_green, _ks_blue;  // Specular reflection

};

