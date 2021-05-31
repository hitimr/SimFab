#pragma once
#include <cmath>
#include <iostream>
#include <vector>

class Vector3D 
{
public:
    double m_x;
    double m_y;
    double m_z;

    Vector3D(){ m_x = 0; m_y = 0; m_z = 0; } 
    Vector3D(double x, double y) :             m_x(x), m_y(y) { m_z = 0; }    // 2D
    Vector3D(double x, double y, double z) :   m_x(x), m_y(y),  m_z(z) {}     // 3D

    inline double norm_l2() const { return sqrt( m_x * m_x + m_y * m_y + m_z * m_z); }
    double norm_inf() const;

    // Operators
    friend Vector3D operator+(const Vector3D & a, const Vector3D & b);   // vector addition
    friend Vector3D operator-(const Vector3D & a, const Vector3D & b);   // vector substraction
    friend Vector3D operator/(const Vector3D & v, double s);             // division by scalar
    friend Vector3D operator*(const Vector3D & v, double s);             // multiplication by scalar
    friend bool     operator==(const Vector3D & a, const Vector3D & b);        // comaprison
    friend std::ostream & operator<<(std::ostream & os, const Vector3D & v);
};



inline Vector3D abs(const Vector3D & v) { return Vector3D(std::abs(v.m_x), std::abs(v.m_y), std::abs(v.m_z)); }
inline double max(double a, double b)   { return a > b ? a : b; }
inline double min(double a, double b)   { return a < b ? a : b; }
double minElement(const std::vector<double> & v);
Vector3D max(const Vector3D & v, double c);
Vector3D max(const Vector3D & v, const Vector3D & w);
Vector3D min(const Vector3D & v, double c);
double l2_norm(const Vector3D & v);