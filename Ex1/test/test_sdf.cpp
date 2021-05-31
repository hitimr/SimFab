#include <iostream>
#include <cmath>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "vector3d.h"
#include "sdf.h"


using namespace std;




int test_constructor()
{
    cout << "Testing constructors..." << endl;

    // Sphere
    char *argv[] = {strdup("test_sdf"), strdup("2"), strdup("2"), strdup("0.1"), strdup("Sphere"), strdup("1"), strdup("1"), strdup("0"), strdup("0.5")}; // strdup required to supress ISO warning

    SDF_Sphere sdf(argv);

    assert(sdf.m_type == TYPE_SPHERE);
    assert(sdf.m_center == Vector3D(1,1,0));
    assert(sdf.m_radius == 0.5);

    cout << "OK!" << endl;
    return SUCCESS;
}


int test_gradient()
{
    cout << "Testing gradient()..." << endl;

    // Sphere
    char *argv1[] = {strdup("test_sdf"), strdup("2"), strdup("2"), strdup("0.1"), strdup("Sphere"), strdup("1"), strdup("1"), strdup("0"), strdup("0.5")}; // strdup required to supress ISO warning
    SDF_Sphere sdf1(argv1);
    assert(sdf1.gradient({1.5, 1.0, 0}) == Vector3D({ 0.5,  0.0, 0}));
    assert(sdf1.gradient({0.5, 1.0, 0}) == Vector3D({-0.5,  0.0, 0}));
    assert(sdf1.gradient({1.0, 1.5, 0}) == Vector3D({ 0.0,  0.5, 0}));
    assert(sdf1.gradient({1.0, 0.5, 0}) == Vector3D({ 0.0, -0.5, 0}));


    // Rectangle
    char *argv2[] = {strdup("test_sdf"), strdup("2"), strdup("2"), strdup("0.1"), strdup("Rectangle"), strdup("0.5"), strdup("0.5"), strdup("1.5"), strdup("1.5")}; // strdup required to supress ISO warning
    SDF_Rectangle sdf2(argv2);
    assert(sdf2.gradient({1.5, 1.0, 0}) == Vector3D({ 0.5,  0.0,  0}));
    assert(sdf2.gradient({0.5, 1.0, 0}) == Vector3D({-0.5,  0.0,  0}));
    assert(sdf2.gradient({1.0, 1.5, 0}) == Vector3D({ 0.0,   0.5, 0}));
    assert(sdf2.gradient({1.0, 0.5, 0}) == Vector3D({ 0.0,  -0.5, 0}));


    cout << "OK!" << endl;
    return SUCCESS;
}


int test_surfaceNormal()
{
    cout << "Testing surfaceNormal()..." << endl;

    
    // Sphere
    char *argv1[] = {strdup("test_sdf"), strdup("2"), strdup("2"), strdup("0.1"), strdup("Sphere"), strdup("1"), strdup("1"), strdup("0"), strdup("0.5")}; // strdup required to supress ISO warning
    SDF_Sphere sdf1(argv1);
    assert(sdf1.surfaceNormal({1.5, 1.0, 0}) == Vector3D({ 1.0,  0.0, 0}));
    assert(sdf1.surfaceNormal({0.5, 1.0, 0}) == Vector3D({-1.0,  0.0, 0}));
    assert(sdf1.surfaceNormal({1.0, 1.5, 0}) == Vector3D({ 0.0,  1.0, 0}));
    assert(sdf1.surfaceNormal({1.0, 0.5, 0}) == Vector3D({ 0.0, -1.0, 0}));


    // Rectangle
    char *argv2[] = {strdup("test_sdf"), strdup("4"), strdup("4"), strdup("0.1"), strdup("Rectangle"), strdup("1"), strdup("1"), strdup("3"), strdup("3")}; // strdup required to supress ISO warning
    SDF_Rectangle sdf2(argv2);
    assert(sdf2.surfaceNormal({2.0, 1.0, 0}) == Vector3D({  0,    -1, 0}));
    assert(sdf2.surfaceNormal({1.0, 2.0, 0}) == Vector3D({ -1,     0, 0}));

    cout << "OK" << endl;
    return SUCCESS;
}




int main()
{
    cout << "Testing sdf..." << endl;

    assert(test_constructor() == SUCCESS);
    //assert(test_gradient() == SUCCESS);
    assert(test_surfaceNormal() == SUCCESS);

    cout << "All Tests in test_sdf passed!" << endl;
    return SUCCESS;
}