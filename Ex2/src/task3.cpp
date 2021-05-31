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
#include <lsToMesh.hpp>

#include "common.hpp"

using namespace std;


// from https://viennatools.github.io/ViennaLS/doxygen/html/SquareEtch_8cpp-example.html#a31
class UniformVerticalEtch : public lsVelocityField<double> {
public:
  double getScalarVelocity(const std::array<double, 3> & /*coordinate*/,
                           int material,
                           const std::array<double, 3> &normalVector,
                           unsigned long /*pointId*/) {
    // if the surface of material 1 is facing upwards, etch it anisotropically
    if (
        material == 1 && 
        //normalVector[0] == 0. && 
        //normalVector[1] == 0 && 
        normalVector[2] > 0 )
    {
      return -std::abs(normalVector[2]);
      //return -1;
    } 
    else
    {
        return 0.;
    }
        
  }
};


class UniformDeposition : public lsVelocityField<double> {
public:
  double getScalarVelocity(const std::array<double, 3> & /*coordinate*/,
                           int material,
                           const std::array<double, 3> &normalVector,
                           unsigned long /*pointId*/) {
    // if the surface of material 1 is facing upwards, etch it anisotropically
    return 1;        
  }
};



int main()
{   
    double extent = 100;
    double gridDelta = 2;

    
    double thickness_mask = 20;
    double thickness_silicon = 50;
    double thickness_oxide = 20;
    double thickness_substrate = 20;
    double thickness_spacer = 5;
    double thickness_gate = 40;

    double width_sourceDrain = 20;

    bool renderAll = true;

    /*
        Levels:


        Silicon
                    -> 0-Level
        Oxide 

        Silicon substrate
    */

    // (0) Create Substrate to match Fig 4
    auto substrate = lsSmartPointer<lsDomain<NumericType, D>>::New(gridDelta);
    {
        double minCorner[] = {-extent, -extent, -thickness_substrate - thickness_oxide};
        double maxCorner[] = {extent, extent, -thickness_oxide};
        lsMakeGeometry<NumericType, D>(substrate, lsSmartPointer<lsBox<NumericType, D>>::New(minCorner, maxCorner)).apply();
    }


    // (1) Oxide Deposition
    auto finFet = lsSmartPointer<lsDomain<NumericType, D>>::New(gridDelta);
    {
        double minCorner[] = {-extent, -extent, -thickness_oxide};
        double maxCorner[] = {extent, extent, 0};
        lsMakeGeometry<NumericType, D>(finFet, lsSmartPointer<lsBox<NumericType, D>>::New(minCorner, maxCorner)).apply();

        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(finFet, mesh).apply();
        lsBooleanOperation<NumericType, D>(finFet, substrate, lsBooleanOperationEnum::UNION).apply();
        if(renderAll) renderMesh(mesh, "task3_1_oxide_deposition", false);
    }

    // (2) Silicon Deposition
    auto silicon = lsSmartPointer<lsDomain<NumericType, D>>::New(gridDelta);
    {
        double minCorner[] = {-extent, -extent, 0};
        double maxCorner[] = {extent, extent, thickness_silicon};
        lsMakeGeometry<NumericType, D>(silicon, lsSmartPointer<lsBox<NumericType, D>>::New(minCorner, maxCorner)).apply();
        lsBooleanOperation<NumericType, D>(finFet, silicon, lsBooleanOperationEnum::UNION).apply();
    
        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(finFet, mesh).apply();
        if(renderAll) renderMesh(mesh, "task3_2_silicon_deposition", false);
    }

    // (3) Mask creation
    auto mask = lsSmartPointer<lsDomain<NumericType, D>>::New(gridDelta);
    {
        double minCorner[] = {-extent, -width_sourceDrain/2, thickness_silicon};
        double maxCorner[] = {extent, width_sourceDrain/2, thickness_silicon + thickness_mask};
        lsMakeGeometry<NumericType, D>(mask, lsSmartPointer<lsBox<NumericType, D>>::New(minCorner, maxCorner)).apply();
        lsBooleanOperation<NumericType, D>(finFet, mask, lsBooleanOperationEnum::UNION).apply();
    
        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(finFet, mesh).apply();
        if(renderAll) renderMesh(mesh, "task3_3_mask_cration", false);
    }


    // (4) Fin Creation
    {
        cout << "Fin creation" << endl;
        lsAdvect<double, D> advectionKernel;
        auto etchVel = lsSmartPointer<UniformVerticalEtch>::New();

        lsAdvect<double, D> etch(etchVel);
        etch.insertNextLevelSet(mask);
        etch.insertNextLevelSet(silicon);
        etch.insertNextLevelSet(substrate);
        etch.insertNextLevelSet(finFet);
        etch.setAdvectionTime(thickness_silicon);
        etch.apply();

        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(finFet, mesh).apply();
        if(renderAll) renderMesh(mesh, "task3_4_fin_creation", false);
    }


    // (5) mask removal
    lsBooleanOperation<NumericType, D>(finFet, mask, lsBooleanOperationEnum::RELATIVE_COMPLEMENT).apply();
    
    auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
    lsToSurfaceMesh<NumericType, D>(finFet, mesh).apply();
    if(renderAll) renderMesh(mesh, "task3_5_maskRemoval", false);


    // (6) Spacer Deposition
    {
        cout << "Depositing spacer" << endl;
        lsAdvect<double, D> advectionKernel;
        auto vel = lsSmartPointer<UniformDeposition>::New();

        lsAdvect<double, D> deposition(vel);
        deposition.insertNextLevelSet(mask);
        deposition.insertNextLevelSet(silicon);
        deposition.insertNextLevelSet(substrate);
        deposition.insertNextLevelSet(finFet);
        deposition.setAdvectionTime(thickness_spacer);
        deposition.apply();

        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(finFet, mesh).apply();
        if(renderAll) renderMesh(mesh, "task3_6_spacerDeposition", false);
    }

    // (7) Gate Deposition
    {
        cout << "Depositing Gate" << endl;
        lsAdvect<double, D> advectionKernel;
        auto vel = lsSmartPointer<UniformDeposition>::New();

        lsAdvect<double, D> deposition(vel);
        deposition.insertNextLevelSet(silicon);
        deposition.insertNextLevelSet(finFet);
        deposition.setAdvectionTime(thickness_gate);
        deposition.apply();

        auto mesh = lsSmartPointer<lsMesh<NumericType>>::New();
        lsToSurfaceMesh<NumericType, D>(finFet, mesh).apply();
        renderMesh(mesh, "task3_7_gateDeposition", false);
    }
}