#pragma once

#include <vector>
#include <string>

#include "common.h"
#include "vector3d.h"


/*******************************************************************************
    Base Class
*******************************************************************************/
class SDF 
{
public: 
    int m_type;
    std::string m_str_type;

    char **m_argv;

    SDF() {};
    SDF(char **argv);

   
    virtual double eval(const Vector3D & c); 
    Vector3D gradient(const Vector3D & v, double eps = EPSILON);
    Vector3D diff(const Vector3D & v, int direction, double eps = EPSILON);
    Vector3D surfaceNormal(const Vector3D & v, double eps = EPSILON);
    double curvature(const Vector3D & v, double eps = EPSILON);
    //void advance_simple(double delta_t);
};


/*******************************************************************************
    Sphere
*******************************************************************************/
class SDF_Sphere : public SDF
{
public:
    SDF_Sphere(char **argv);

    Vector3D m_center;
    double m_radius;

    double eval(const Vector3D &);
};


/*******************************************************************************
    Recangle (Axis Aligned)
*******************************************************************************/
class  SDF_Rectangle : public SDF
{
public:
    SDF_Rectangle(char **argv);

    Vector3D m_minCorner;   // bottom left
    Vector3D m_maxCorner;   // top right

    double eval(const Vector3D &);
};


