#include <assert.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>


#include "common.h"
#include "sdf.h"
#include "vector3d.h"


using namespace std;

/*******************************************************************************
    Base Class
*******************************************************************************/
SDF::SDF(char **argv) : m_argv(argv)
{
    m_str_type = "no_type";
}

double SDF::eval(const Vector3D & c)
{
    return c.m_x;   // supress Wextra warning
}

Vector3D SDF::gradient(const Vector3D & p, double eps)
{
    return Vector3D(
        eval(p + Vector3D(eps, 0, 0)) - eval(p - Vector3D(eps, 0, 0)),
        eval(p + Vector3D(0, eps, 0)) - eval(p - Vector3D(0, eps, 0)),
        eval(p + Vector3D(0, 0, eps)) - eval(p - Vector3D(0, 0, eps))
    ) / (2 * eps);
}

// foward/backward difference
Vector3D SDF::diff(const Vector3D & v, int direction, double eps)
{
    double phi = eval(v);
    switch(direction)
    {
        case FORWARD:
            return Vector3D(
                eval(v + Vector3D(eps, 0, 0)) - phi,
                eval(v + Vector3D(0, eps, 0)) - phi,
                eval(v + Vector3D(0, 0, eps)) - phi
            ) / eps;

        case BACKWARD:
            return Vector3D(
                (-1.0)*(eval(v - Vector3D(eps, 0, 0)) - phi),
                (-1.0)*(eval(v - Vector3D(0, eps, 0)) - phi),
                (-1.0)*(eval(v - Vector3D(0, 0, eps)) - phi)
            ) / eps;
        default:
            assert(false && "Invaid direction");
            return v;
    }  
}



Vector3D SDF::surfaceNormal(const Vector3D & v, double eps)
{
    Vector3D grad = gradient(v, eps);
    return grad / grad.norm_l2();
}

double SDF::curvature(const Vector3D & v, double eps)
{
    Vector3D n = gradient(surfaceNormal(v, eps));

    return (n.m_x + n.m_y + n.m_z); // TOD: test
}



/*******************************************************************************
    Sphere
*******************************************************************************/
SDF_Sphere::SDF_Sphere(char **argv)
{
    m_type = TYPE_SPHERE;
    m_str_type = "Sphere";

    // parse Arguments
    m_center = {
        atof(argv[ARG_SPHERE_CENTER_X]),
        atof(argv[ARG_SPHERE_CENTER_Y]),
        atof(argv[ARG_SPHERE_CENTER_Z])
    };

    m_radius = atof(argv[ARG_SPHERE_RADIUS]);
}


double SDF_Sphere::eval(const Vector3D & pos)
{
    Vector3D v = pos - m_center;
    return v.norm_l2() - m_radius;
}




/*******************************************************************************
    Rectangle
*******************************************************************************/
SDF_Rectangle::SDF_Rectangle(char **argv)
{ 
    m_type = TYPE_RECT; 
    m_str_type = "Rectangle";

    // Parse Arguments
    m_minCorner = {
        atof(argv[ARG_RECT_MINCORNER_X]),
        atof(argv[ARG_RECT_MINCORNER_Y]),
        0
    };

    m_maxCorner = {
        atof(argv[ARG_RECT_MAXCORNER_X]),
        atof(argv[ARG_RECT_MAXCORNER_Y]),
        0
    };
} 


double SDF_Rectangle::eval(const Vector3D & p)
{
    // Inspired by 
    //https://stackoverflow.com/questions/30545052/calculate-signed-distance-between-point-and-rectangle/30545544


    // calculate the signed distance from x and y borders
    // Taking the maximum of these values gives the signed distance to the closest border
    double dx =  max(m_minCorner.m_x - p.m_x, p.m_x - m_maxCorner.m_x);
    double dy =  max(m_minCorner.m_y - p.m_y, p.m_y - m_maxCorner.m_y);


    // If both values are negative, take the maximum (i.e. closest to a border).
    if((dx < 0) && (dy < 0))
    {
        // we are inside the rectangle
        return min(0, max(dx, dy));
    }

    // If both values are positive, the closest point is a corner, in which case we want the length.
    if((dx > 0) && (dy > 0))
    {

        return sqrt(dx * dx + dy * dy + p.m_z * p.m_z);
    }

    // outside of the rectangle but closer to an border than a corner
    return max(dx, dy);
}



