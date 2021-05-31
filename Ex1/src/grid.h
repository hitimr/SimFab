#pragma once
#include <vector>
#include <string>

#include "sdf.h"
#include "vector3d.h"



/**
 * Grid that defines the simulation domain
 * 
 * Data is stored as a 2D array of floats. Each containing the value of the SDF
 * at a certain point.
*/
class Grid 
{
public:    
    double m_x_size;
    double m_y_size;
    double m_delta;

    std::vector<int> m_border_types;
    std::vector<std::vector<double>> m_data;
    std::vector<std::vector<double>> m_velocity_field;
    SDF * m_sdf;

    Grid() {};

    Grid(char *argv[]);

    std::vector<int> parse_border_types(char *argv);
    void write(std::string fileName);
    void add_surface(SDF * sdf);
    void update();
    void advance_simple(double velocity, double delta_t=1);
    void advance_engquist_osher(double velocity, double delta_t=1);
    void advance_with_normal_vector(const Vector3D & direction, double delta_t);
    void advance_with_curvature(double delta_t);
    double advance_engquist_osher_single(int x, int y, double velocity, double delta_t);
    Vector3D to_realCoords(const Vector3D & v);
};