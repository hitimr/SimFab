#include <iostream>
#include <assert.h>
#include <string>
#include <string.h>
#include <cmath>

#include "common.h"
#include "grid.h"
#include "vector3d.h"
#include VTKOUTPUT_H

using namespace std;


Grid::Grid(char *argv[])
{
    m_x_size =    atof(argv[ARG_XSIZE]);      // TODO: change to float
    m_y_size =    atof(argv[ARG_YSIZE]);     
    m_delta =     atof(argv[ARG_SPACING]);
    m_border_types = parse_border_types(&argv[ARG_BORDER_TYPES][0]);

    assert(m_x_size > 0);
    assert(m_y_size > 0);
    assert(m_border_types.size() == 4);   // We only have square grids
    assert(m_delta > 0 && "ERROR: Invalid value for Argument ARG_SPACING (delta). Value must be >0");

    m_data = vector<vector<double>>(m_x_size / m_delta + 1, vector<double>(m_y_size / m_delta + 1 , 0));    // initialize grid with 0
    m_velocity_field = vector<vector<double>>(m_x_size / m_delta + 1, vector<double>(m_y_size / m_delta + 1 , 0)); // initialize velocity field with 0
}

// TODO:" unit test"
std::vector<int> Grid::parse_border_types(char *argv)
{
    string arg(argv);

    vector<int> border_types(4,-1);
    
    for(int i = 0; i < 4; i++)
    {        
        if(arg[i] == 'p')
            border_types[i] = BORDER_PERIODIC;
        else if(arg[i] == 'r')
            border_types[i] = BORDER_REFLECTIVE;
        else
            assert(false && "Invalid border type");
            
    }

    return border_types;
}

void Grid::write(string fileName)
{
    auto out = vtkOutput(fileName, m_delta, m_data);
    out.write();
}

void Grid::add_surface(SDF * sdf)
{
    assert(sdf->m_type != TYPE_NOTYPE);

    cout << "Adding " << (sdf->m_str_type) << " to grid..." << endl;
    m_sdf = sdf;
}

void Grid::update()
{
    // TODO: parallelize?    
    // TODO: pull x*m_delta out. same for y
    for(size_t y = 0; y < m_data[0].size(); y++)
    {
        for(size_t x = 0; x < m_data.size(); x++)
        {    
            // SDF Value without consideration of borders       
            double center_val = m_sdf->eval({x * m_delta, y * m_delta, 0});
            vector<double> values(9, center_val);
            
            // SDF Values depending on the kind of border
            if(m_border_types[BOTTOM] == BORDER_PERIODIC)
                values[BOTTOM] = m_sdf->eval({ (x * m_delta), (y * m_delta) - m_y_size, 0});

            if(m_border_types[RIGHT] == BORDER_PERIODIC) 
                values[RIGHT] = m_sdf->eval({ (x * m_delta) - m_x_size, (y * m_delta), 0});

            if(m_border_types[TOP] == BORDER_PERIODIC)
                values[TOP] = m_sdf->eval({ (x * m_delta), (y * m_delta) + m_y_size, 0});

            if(m_border_types[LEFT] == BORDER_PERIODIC) 
                values[LEFT] = m_sdf->eval({ (x * m_delta) + m_x_size, (y * m_delta), 0});

            if((m_border_types[BOTTOM] == BORDER_PERIODIC) && (m_border_types[LEFT] == BORDER_PERIODIC) )
                values[BOTTOM_LEFT] =  m_sdf->eval({ (x * m_delta) + m_x_size, (y * m_delta) - m_y_size, 0});

            if((m_border_types[BOTTOM] && m_border_types[RIGHT]) == BORDER_PERIODIC)
                values[BOTTOM_RIGHT] =  m_sdf->eval({ (x * m_delta) - m_x_size, (y * m_delta) - m_y_size, 0});

            if((m_border_types[TOP] && m_border_types[LEFT]) == BORDER_PERIODIC)
                values[TOP_RIGHT] =  m_sdf->eval({ (x * m_delta) + m_x_size, (y * m_delta) + m_y_size, 0});

            if((m_border_types[TOP] && m_border_types[RIGHT]) == BORDER_PERIODIC)
                values[TOP_LEFT] =  m_sdf->eval({ (x * m_delta) - m_x_size, (y * m_delta) + m_y_size, 0});


            m_data[x][y] = minElement(values) - m_velocity_field[x][y];
        } 
    }
}


void Grid::advance_simple(double velocity, double delta_t)
{
    double distance = delta_t*velocity;
    for(size_t y = 0; y < m_velocity_field[0].size(); y++)
    {
        for(size_t x = 0; x < m_velocity_field.size(); x++)
        {  
            m_velocity_field[x][y] += distance;
        }
    }
}


void Grid::advance_engquist_osher(double velocity, double delta_t)
{
  
    for(size_t y = 0; y < m_velocity_field[0].size(); y++)
    {
        for(size_t x = 0; x < m_velocity_field.size(); x++)
        {  
            m_velocity_field[x][y] = advance_engquist_osher_single(x, y, velocity, delta_t);
        }
    }   
}

double Grid::advance_engquist_osher_single(int x, int y, double velocity, double delta_t)
{
    double distance = delta_t*velocity;
    Vector3D pos = {x * m_delta, y * m_delta, 0};
    double V =  distance * l2_norm(m_sdf->gradient(pos));

    Vector3D forward_diff = m_sdf->diff(pos, FORWARD);
    Vector3D backward_diff = m_sdf->diff(pos, BACKWARD);
    double enquist_osher_factor;
    if(V <= 0)
    {
        enquist_osher_factor = sqrt(
            pow(max((-1.0) * backward_diff.m_x, 0), 2) + pow(min((-1.0) * forward_diff.m_x, 0), 2) +
            pow(max((-1.0) * backward_diff.m_y, 0), 2) + pow(min((-1.0) * forward_diff.m_y, 0), 2)
        );
    }
    else
    {
        enquist_osher_factor = sqrt(
            pow(max(backward_diff.m_x, 0), 2) + pow(min(forward_diff.m_x, 0), 2) +
            pow(max(backward_diff.m_y, 0), 2) + pow(min(forward_diff.m_y, 0), 2)
        );
    }


    if(abs(V) <= m_delta / delta_t)
    {
        return distance * enquist_osher_factor;
    }
    else
    {
        return advance_engquist_osher_single(x, y, velocity, delta_t/2.0)*2;
    }
}


void Grid::advance_with_normal_vector(const Vector3D & direction, double delta_t)
{
    for(size_t y = 0; y < m_velocity_field[0].size(); y++)
    {
        for(size_t x = 0; x < m_velocity_field.size(); x++)
        {  
            Vector3D pos = {x * m_x_size, y * m_y_size, 0};
            Vector3D n = m_sdf->surfaceNormal(pos);
            m_velocity_field[x][y] = advance_engquist_osher_single(x,y, 1, delta_t)*(direction.m_x * n.m_x + direction.m_y * n.m_y + direction.m_z * n.m_z);
        }
    }      
}


void Grid::advance_with_curvature(double delta_t)
{
    for(size_t y = 0; y < m_velocity_field[0].size(); y++)
    {
        for(size_t x = 0; x < m_velocity_field.size(); x++)
        {  
            Vector3D pos = {x * m_x_size, y * m_y_size, 0};
            double velocity = -1.0*m_sdf->curvature(pos);
            m_velocity_field[x][y] = velocity * delta_t;
        }
    }   
}