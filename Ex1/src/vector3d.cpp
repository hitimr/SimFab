#include <vector>
#include <assert.h>
#include "vector3d.h"

using namespace std;

double Vector3D::norm_inf() const
{
    return  m_x > m_y ? 
        m_x > m_z ? m_x : m_z : 
        m_y > m_z ? m_y : m_z;
}


Vector3D operator+(const Vector3D & a, const Vector3D & b)
{
    return Vector3D(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
}


Vector3D operator-(const Vector3D & a, const Vector3D & b)
{
    return Vector3D(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
}


Vector3D operator/(const Vector3D & v, double s)
{
    return Vector3D(v.m_x / s, v.m_y / s, v.m_z / s);
}

Vector3D operator*(const Vector3D & v, double s)
{
    return Vector3D(v.m_x * s, v.m_y * s, v.m_z * s);
}



bool operator==(const Vector3D & a, const Vector3D & b)
{
    if((a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_z == b.m_z)) 
        return true;
    else 
        return false;
}


ostream& operator<<(ostream& os, const Vector3D& v)
{
    os << "[" << v.m_x << ", " << v.m_y << ", " << v.m_z << "]";
    return os;
}

// returns the smallest elemet in a vector
double minElement(const vector<double> & v)
{
    assert(v.size() > 0 && "double min(vector<double> v) requries a vector of size >0!");
    double minVal = v[0];

    for(size_t i = 1; i < v.size(); i++)
    {
        if(v[i] < minVal)
            minVal = v[i];
    }

    return minVal;
}


Vector3D max(const Vector3D & v, double c)
{
    return Vector3D(
        max(v.m_x, c),
        max(v.m_y, c),
        max(v.m_z, c)
    );
}


Vector3D max(const Vector3D & v, const Vector3D & w)
{ 
    return Vector3D(
        max(v.m_x, w.m_x),
        max(v.m_y, w.m_y),
        max(v.m_z, w.m_z)
    );
}


Vector3D min(const Vector3D & v, double c)
{
    return Vector3D(
        min(v.m_x, c),
        min(v.m_y, c),
        min(v.m_z, c)
    );
}


double l2_norm(const Vector3D & v)
{
    return sqrt( 
        v.m_x * v.m_x + 
        v.m_y * v.m_y + 
        v.m_z * v.m_z
    );
}
