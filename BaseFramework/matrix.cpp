#include "matrix.h"
//#include <cmath>


Matrix::Matrix() : _m{ 0 }
{
    /*
    _m[0][0] = 0;
    _m[0][1] = 0;
    _m[1][0] = 0;
    _m[1][1] = 0;
    */
}

Matrix::Matrix(std::initializer_list<float> list) : _m{ 0 }
{
    if (list.size() != ROWS * COLS)
    {
        throw "Insufficiant values supplied";
    }
    auto iterator = list.begin();
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            _m[i][j] = *iterator++;
        }
    }

}

Matrix::Matrix(const Matrix& other)
{
    Copy(other);
}

// Destructor
Matrix::~Matrix()
{
}

float Matrix::GetM(const int row, const int column) const
{
    return _m[row][column];
}

void Matrix::SetM(const int row, const int column, const float value)
{
    _m[row][column] = value;
}

Matrix& Matrix::operator=(const Matrix& rhs)
{
    // TODO: insert return statement here

    if (this != &rhs)
    {
        Copy(rhs);
    }
    return *this;
}


bool Matrix::operator==(const Matrix& other) const
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (_m[i][j] != other.GetM(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

const Matrix Matrix::operator*(const Matrix& other) const
{
    Matrix result;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            float resultValue = 0;
            for (int k = 0; k < ROWS; k++)
            {
                resultValue += _m[i][k] * other.GetM(k, j);
            }
            result.SetM(i, j, resultValue);
        }
    }
    return result;
}

const Vertex Matrix::operator*(const Vertex& p) const
{
    Vertex newVertex(p);

    newVertex.SetX(_m[0][0] * p.GetX() + _m[0][1] * p.GetY() + _m[0][2] * p.GetZ() + _m[0][3] * p.GetW());
    newVertex.SetY(_m[1][0] * p.GetX() + _m[1][1] * p.GetY() + _m[1][2] * p.GetZ() + _m[1][3] * p.GetW());
    newVertex.SetZ(_m[2][0] * p.GetX() + _m[2][1] * p.GetY() + _m[2][2] * p.GetZ() + _m[2][3] * p.GetW());
    newVertex.SetW(_m[3][0] * p.GetX() + _m[3][1] * p.GetY() + _m[3][2] * p.GetZ() + _m[3][3] * p.GetW());


    return newVertex;
}

void Matrix::Copy(const Matrix& other)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            _m[i][j] = other.GetM(i, j);
        }
    }
}
