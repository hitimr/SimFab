#include <array>
#include <iostream>
#include <string>
#include <assert.h>

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
#include <lsToMesh.hpp>
#include <random>

#include "common.hpp"

using namespace std;


// from https://viennatools.github.io/ViennaLS/doxygen/html/SquareEtch_8cpp-example.html#a31
class Etch_Vel : public lsVelocityField<double> {
private:
    double m_velocity_substrate;
    double m_velocity_passivationLayer;
    double m_randomNess;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution;

public:
    Etch_Vel(double velocity_substrate, double velocity_passivationLayer, double randomNess)
    {
        assert(velocity_substrate >= 0);
        assert(velocity_passivationLayer >= 0);

        m_velocity_substrate = velocity_substrate;
        m_velocity_passivationLayer = velocity_passivationLayer;
        m_randomNess = randomNess;

        distribution = normal_distribution<double>(0, 1);
    }

    double getScalarVelocity(const std::array<double, 3> & /*coordinate*/,
                           int material,
                           const std::array<double, 3> &normalVector,
                           unsigned long /*pointId*/) {
    // if the surface of material 1 is facing upwards, etch it anisotropically
    double vel;
    switch(material)
    {
        case 0:     
            return 0;   // Mask

        case 1:     // substrate
            vel = (-1.)*m_velocity_substrate;
            vel += vel*m_randomNess*distribution(generator);
            return vel;

        default:    // passivation layer
            if(normalVector[2] > 0 )
            {
                vel = (-1)*m_velocity_passivationLayer*std::abs(normalVector[2]);
                vel += vel*m_randomNess*distribution(generator);
                return vel;
            }
            else
            {
                return 0;
            }
    }        
  }
};



// from https://viennatools.github.io/ViennaLS/doxygen/html/SquareEtch_8cpp-example.html#a31
class Deposition_Vel : public lsVelocityField<double> {
public:
    double getScalarVelocity(const std::array<double, 3> & /*coordinate*/,
                            int material,
                            const std::array<double, 3> &normalVector,
                            unsigned long /*pointId*/) {
   
        
        switch(material)
        {
            case  0: return 1;  // perfect isotropic deposition
            case  1: return 1;
            default: return 0;  // do not deposit where there is already material     
        }
        
    }
};



// Doc: https://viennatools.github.io/ViennaLS/doxygen/html/index.html
int DRIE_etch(
    double etch_time, 
    double etch_velocity_substrate,
    double etch_velocity_passivatioLayer,
    double deposition_time, 
    int steps,
    double randomNess,
    string fileName)
{   
    cout << "Simulating " << fileName << "..." << endl;
    // Example: https://viennatools.github.io/ViennaLS/doxygen/html/PeriodicBoundary_8cpp-example.html
    omp_set_num_threads(4);

    NumericType extent = 40 MICROMETERS;
    NumericType gridDelta = 1 MICROMETERS;
    double mask_thickness = 10 MICROMETERS;

    double bounds[2 * D] = {-extent, extent, -extent, extent, -extent, extent};
    lsDomain<NumericType, D>::BoundaryType boundaryCons[D];
    for (unsigned i = 0; i < D - 1; ++i)
        boundaryCons[i] = lsDomain<NumericType, D>::BoundaryType::REFLECTIVE_BOUNDARY;
    boundaryCons[2] = lsDomain<NumericType, D>::BoundaryType::INFINITE_BOUNDARY;

    // Substrate
    auto substrate = lsSmartPointer<lsDomain<NumericType, D>>::New(bounds, boundaryCons, gridDelta);
    {
        double minCorner[] = {-extent, -extent, -extent};
        double maxCorner[] = {extent, extent, 0.};
        auto box = lsSmartPointer<lsBox<NumericType, D>>::New(minCorner, maxCorner);
        lsMakeGeometry<NumericType, D>(substrate, box).apply();
    }


    // add mask
    auto mask = lsSmartPointer<lsDomain<NumericType, D>>::New(bounds, boundaryCons, gridDelta);
    {
        // make -x and +x greater than domain for numerical stability
        double minCorner[] = {-extent, -extent, 0. MICROMETERS};
        double maxCorner[] = {extent, extent, 10. MICROMETERS};
        auto geometry = lsSmartPointer<lsBox<NumericType, D>>::New(minCorner, maxCorner);
        lsMakeGeometry<NumericType, D>(mask, geometry).apply();
        lsBooleanOperation<NumericType, D>(substrate, mask, lsBooleanOperationEnum::UNION).apply();
    }

    {
        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(substrate, mesh).apply();
        renderMesh(mesh, "task2.2_mask", false);
    }


    // cut a circular holer into the mask
    auto cylinder = lsSmartPointer<lsDomain<NumericType, D>>::New(bounds, boundaryCons, gridDelta);
    {
        double origin[] = {0, 0, 0};
        double direction[] = {0, 0, 1 MICROMETERS};
        double height = 10 MICROMETERS;
        double radius = 30 MICROMETERS;
        auto geometry = lsSmartPointer<lsCylinder<NumericType, D>>::New(origin, direction, height, radius);
        lsMakeGeometry<NumericType, D>(cylinder, geometry).apply();
        lsBooleanOperation<NumericType, D>(substrate, cylinder, lsBooleanOperationEnum::RELATIVE_COMPLEMENT).apply();
    }

    {
        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(substrate, mesh).apply();
        renderMesh(mesh, "task2.2_circularHole", false);
    }


    auto etch_vel = lsSmartPointer<Etch_Vel>::New(etch_velocity_substrate, etch_velocity_passivatioLayer,randomNess);
    lsAdvect<double, D> etch(etch_vel);
    etch.insertNextLevelSet(mask);
    etch.insertNextLevelSet(substrate);
    etch.setAdvectionTime(etch_time);
  
    auto deposition_vel = lsSmartPointer<Deposition_Vel>::New();
    lsAdvect<double, D> deposition(deposition_vel);
    deposition.insertNextLevelSet(mask);
    deposition.insertNextLevelSet(substrate);
    deposition.setAdvectionTime(deposition_time);

    etch.apply();

    for(int i = 0; i < steps; i++)
    {
        cout << "Step "<< i << "/" << steps << endl;
        auto newLayer = lsSmartPointer<lsDomain<NumericType, D>>::New(substrate);
        deposition.insertNextLevelSet(newLayer);
        deposition.apply();
        etch.insertNextLevelSet(newLayer);
        etch.apply();
    }


    auto cut = lsSmartPointer<lsDomain<NumericType, D>>::New(bounds, boundaryCons, gridDelta);
    {
        // make -x and +x greater than domain for numerical stability
        double minCorner[] = {0,0,-extent};
        double maxCorner[] = {extent, extent, extent};
        auto geometry = lsSmartPointer<lsBox<NumericType, D>>::New(minCorner, maxCorner);
        lsMakeGeometry<NumericType, D>(cut, geometry).apply();
        lsBooleanOperation<NumericType, D>(substrate, cut, lsBooleanOperationEnum::RELATIVE_COMPLEMENT).apply();
    }
    
    {
        // render
        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(substrate, mesh).apply();
        renderMesh(mesh, fileName, false);
    }
    
    return 0;
}

int main()
{
    /*
    Args:
        double etch_time, 
        double etch_velocity_substrate,
        double etch_velocity_passivatioLayer,
        double deposition_time, 
        int steps,
        double randomNess,
    *//*
    DRIE_etch(3, 1, 0.25, 2.5, 10, 0, "task2.2_uniformLong");
    DRIE_etch(3, 1, 0.25, 2.5, 10, 0.4, "task2.2_randomLong");
    DRIE_etch(1.5, 1, 0.5, 2.5, 10, 0, "task2.2_uniformShort");
    DRIE_etch(1.5, 1, 0.5, 2.5, 10, 0.4, "task2.2_randomShort");*/
    DRIE_etch(2.5, 1, 0.5, 2.5, 10, 0, "task2.2_uniformSkewed");
    DRIE_etch(2.5, 1, 0.5, 2.5, 10, 0.4, "task2.2_randomSkewed");

    return 0;
}