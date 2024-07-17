// Polygon3D.cpp
#include "Polygon3D.h"

Polygon3D::Polygon3D() : _RGBValue(RGB(0, 0, 0))
{
    _indices[0] = 0;
    _indices[1] = 0;
    _indices[2] = 0;
    _normal = Vector(0, 0, 0);
    _cullState = false;
    _averageZ = 0.0f;
}

Polygon3D::Polygon3D(int index0, int index1, int index2) : _RGBValue(RGB(0, 0, 0))
{
    _indices[0] = index0;
    _indices[1] = index1;
    _indices[2] = index2;
    _normal = Vector(0, 0, 0);
    _cullState = false;
    _averageZ = 0.0f;
}

Polygon3D::~Polygon3D()
{
}

Polygon3D::Polygon3D(const Polygon3D& p) : _RGBValue(p._RGBValue)
{
    _indices[0] = p.GetIndex(0);
    _indices[1] = p.GetIndex(1);
    _indices[2] = p.GetIndex(2);
    _normal = p.GetNormal();
    _cullState = p.GetCullState();
    _averageZ = p.GetAverageZ();
}

int Polygon3D::GetIndex(int x) const
{
    return _indices[x];
}

Polygon3D& Polygon3D::operator=(const Polygon3D& rhs)
{
    if (this != &rhs)
    {
        _indices[0] = rhs.GetIndex(0);
        _indices[1] = rhs.GetIndex(1);
        _indices[2] = rhs.GetIndex(2);
        _normal = rhs.GetNormal();
        _cullState = rhs.GetCullState();
        _averageZ = rhs.GetAverageZ();
        _RGBValue = rhs._RGBValue;
    }
    return *this;
}

void Polygon3D::SetCullState(bool state)
{
    _cullState = state;
}

bool Polygon3D::GetCullState() const
{
    return _cullState;
}

void Polygon3D::SetNormal(const Vector& normal)
{
    _normal = normal;
}

const Vector& Polygon3D::GetNormal() const
{
    return _normal;
}

float Polygon3D::GetAverageZ() const
{
    return _averageZ;
}

void Polygon3D::SetAverageZ(float averagePolygonZ)
{
    _averageZ = averagePolygonZ;
}

COLORREF Polygon3D::GetPolygon3dColor() const
{
    return _RGBValue;
}

void Polygon3D::SetPolygon3dColor(COLORREF RGBValue)
{
    _RGBValue = RGBValue;
}