// Polygon3D.h
#pragma once
#include "Vector.h"
#include <Windows.h>

class Polygon3D
{
public:
    Polygon3D();
    Polygon3D(int index0, int index1, int index2);
    ~Polygon3D();
    Polygon3D(const Polygon3D& p);

    int GetIndex(int) const;
    Polygon3D& operator=(const Polygon3D& rhs);
    void SetCullState(bool state);
    bool GetCullState() const;
    void SetNormal(const Vector& normal);
    const Vector& GetNormal() const;
    float GetAverageZ() const;
    void SetAverageZ(float averagePolygonZ);
    COLORREF GetPolygon3dColor() const;
    void SetPolygon3dColor(COLORREF RGBValue);

private:
    bool _cullState;
    Vector _normal;
    int _indices[3];
    float _averageZ;
    COLORREF _RGBValue;
};