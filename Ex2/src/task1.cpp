#include <array>
#include <iostream>
#include <string>

#include <lsAdvect.hpp>
#include <lsBooleanOperation.hpp>
#include <lsDomain.hpp>
#include <lsExpand.hpp>
#include <lsGeometries.hpp>
#include <lsMakeGeometry.hpp>
#include <lsPrune.hpp>
#include <lsSmartPointer.hpp>
#include <lsToSurfaceMesh.hpp>
#include <lsVTKWriter.hpp>

#include "common.hpp"

using namespace std;



// from https://viennatools.github.io/ViennaLS/doxygen/html/SquareEtch_8cpp-example.html#a31
class Melt_Vel : public lsVelocityField<double> {
public:
    double getScalarVelocity(const std::array<double, 3> & /*coordinate*/,
                            int material,
                            const std::array<double, 3> &normalVector,
                            unsigned long /*pointId*/) {
   
        
        switch(material)
        {
            case  0: return -2;  
            case  1: return -1;
            default: return 0;
        }
        
    }
};


void task1_3()
{
    NumericType gridDelta = 0.25;

    // start with lower Body
    auto snowMan = lsSmartPointer<lsDomain<NumericType, D>>::New(gridDelta);
    {
        double origin[] = {0, 0, 0};
        double radius = 5;
        auto box = lsSmartPointer<lsSphere<NumericType, D>>::New(origin, radius);
        lsMakeGeometry<NumericType, D>(snowMan, box).apply();
    }

    auto upperBody = lsSmartPointer<lsDomain<NumericType, D>>::New(gridDelta);
    {
        // make -x and +x greater than domain for numerical stability
        double origin[] = {0, 0, 6.5};
        double radius = 3.5;
        auto geometry = lsSmartPointer<lsSphere<NumericType, D>>::New(origin, radius);
        lsMakeGeometry<NumericType, D>(upperBody, geometry).apply();
        lsBooleanOperation<NumericType, D>(snowMan, upperBody, lsBooleanOperationEnum::UNION).apply();
    }

    auto head = lsSmartPointer<lsDomain<NumericType, D>>::New(gridDelta);
    {
        // make -x and +x greater than domain for numerical stability
        double origin[] = {0, 0, 11};
        double radius = 2.5;
        auto geometry = lsSmartPointer<lsSphere<NumericType, D>>::New(origin, radius);
        lsMakeGeometry<NumericType, D>(upperBody, geometry).apply();
        lsBooleanOperation<NumericType, D>(snowMan, upperBody, lsBooleanOperationEnum::UNION).apply();
    }

    {
        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(snowMan, mesh).apply();
        renderMesh(mesh, "task1.3_snowman1", false);
    }

    // Melt snowman
    auto etch_vel = lsSmartPointer<Melt_Vel>::New();
    lsAdvect<double, D> etch(etch_vel);
    etch.insertNextLevelSet(snowMan);
    etch.setAdvectionTime(0.5);
    etch.apply();

    {
        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(snowMan, mesh).apply();
        renderMesh(mesh, "task1.3_snowman2", false);
    }


}


int main()
{
    // general stuff
    omp_set_num_threads(4);
    double gridDelta = 0.25;
    

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Task 1.1
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // create sphere
    auto sphere1 = lsSmartPointer<lsDomain<NumericType, D>>::New(gridDelta);
    {
        NumericType origin[3] = {0., 0., 0.};
        NumericType radius = 10.0;
        lsMakeGeometry<NumericType, D>(sphere1, lsSmartPointer<lsSphere<NumericType, D>>::New(origin, radius)).apply();  
    }
 
    // extract surface
    auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
    lsToSurfaceMesh<NumericType, D>(sphere1, mesh).apply();
    renderMesh(mesh, "task1.1_sphere1", false);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Task 1.2
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    cout << "Starting advection" << endl;
    lsAdvect<double, D> advectionKernel;
    auto constant_vf = lsSmartPointer<ConstantVelocityField>::New(-2);

    advectionKernel.insertNextLevelSet(sphere1);
    advectionKernel.setVelocityField(constant_vf);
    advectionKernel.setAdvectionTime(1);
    advectionKernel.apply();


    double advectionSteps = advectionKernel.getNumberOfTimeSteps();
    std::cout << "Number of Advection steps taken: " << advectionSteps << endl;


    mesh = lsSmartPointer<lsMesh<NumericType>>::New();
    lsToSurfaceMesh<NumericType, D>(sphere1, mesh).apply();
    renderMesh(mesh, "task1.1_sphere2", false);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Task 1.3
     *      Multiple Materials
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    // add additional spheres
    task1_3();
    cout << "Done!" << endl;
    return 0; 
}