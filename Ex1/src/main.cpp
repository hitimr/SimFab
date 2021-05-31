#include <iostream>
#include <assert.h>
#include <string>
#include <ctype.h>

#include "common.h"
#include "grid.h"
#include "sdf.h"


using namespace std;


int main(int argc, char *argv[])
{
    // Parse arguments
    // TODO: assertion for argv size 
    string str_type =  argv[ARG_TYPE];    
    int type = TYPE_NOTYPE;
    if(str_type == "Sphere") type = TYPE_SPHERE;
    else if (str_type == "Rectangle") type = TYPE_RECT;


    string fileName;
    if(argc == ARG_FILE_NAME + 1) fileName = argv[ARG_FILE_NAME];
    else fileName = "out/grid.vtk"; // if no fileName is specified use default name


    // sanity checks
    assert(type != TYPE_NOTYPE && "ERROR: Unknown value for Argument ARG_TYPE. Arguments are case sensitive!");
  
    // Create SDF Object
    SDF * sdf;
    switch(type)
    {
        case TYPE_SPHERE:
            sdf = new SDF_Sphere(argv);
            break;
        
        case TYPE_RECT:
            sdf = new SDF_Rectangle(argv);
            break;

        default:
            assert(false && "Invalid value for type");
    }    


    // Create grid and add sdf object 
    Grid grid(argv);   
    grid.add_surface(sdf);
    grid.update(); 


    // Task 2
    if(argc == ARG_COORD_Y + 1)
    {
        double x = stof(argv[ARG_COORD_X]);
        double y = stof(argv[ARG_COORD_Y]);
        Vector3D pos = {x, y, 0};

        cout << "Parsed point (" << x << "|" << y << ")" << endl;
        cout << "Gradient=" << sdf->gradient(pos) << endl;
        cout << "Surface Normal=" << sdf->surfaceNormal(pos) << endl;
        cout << "Curvature=" << sdf->curvature(pos) << endl;
    }


    // Task 3 - Part 1

    /*
    double V = 10;
    // Task 3.1 simple surface advancement
    // t = 0.1
    grid.advance_simple(V, 0.1);
    grid.update();
    grid.write("out/gridt1.vtk");
    grid.advance_simple(-V, 0.1);
    grid.update();

    // t = 1
    grid.advance_simple(V, 1);
    grid.update();
    grid.write("out/gridt2.vtk");
    grid.advance_simple(-V, 1);
    grid.update();


    // t = 0.1
    grid.advance_engquist_osher(V, 0.1);
    grid.update();
    grid.write("out/gridt1_eo.vtk");
    grid.advance_engquist_osher(-V, 0.1);
    grid.update();

    // t = 1
    grid.advance_engquist_osher(V, 1);
    grid.update();
    grid.write("out/gridt2_eo.vtk");
    grid.advance_engquist_osher(-V, 1);
    grid.update();
    */
   /*
    // Task 3.1 - Part 2 (normal vector velocity)
    grid.advance_with_normal_vector({1,0,0}, 0);
    grid.update();    
    grid.write("results/task3/normal_velocity_t0.vtk");
    grid.advance_with_normal_vector({1,0,0}, 5);
    grid.update();    
    grid.write("results/task3/normal_velocity_t1.vtk");
    grid.advance_with_normal_vector({1,0,0}, 10);
    grid.update();    
    grid.write("results/task3/normal_velocity_t2.vtk");
    grid.advance_with_normal_vector({1,0,0}, 15);
    grid.update();    
    grid.write("results/task3/normal_velocity_t3.vtk");
    */

   // Task 3 - Part 2

   /*
    grid.update();
    grid.write("results/task3/rect_curvature_t0.vtk");
    grid.advance_with_curvature(2);
    grid.update();
    grid.write("results/task3/rect_curvature_t1.vtk");
        grid.advance_with_curvature(4);
    grid.update();
    grid.write("results/task3/rect_curvature_t2.vtk");
        grid.advance_with_curvature(6);
    grid.update();
    grid.write("results/task3/rect_curvature_t3.vtk");
        grid.advance_with_curvature(8);
    grid.update();
    grid.write("results/task3/rect_curvature_t4.vtk");
    */
    cout << "Done" << endl;
    return 0;
}