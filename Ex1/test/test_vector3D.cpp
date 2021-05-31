#include <iostream>
#include <cmath>
#include "assert.h"

#include "common.h"
#include "vector3d.h"

using namespace std;


int test_norms()
{
    assert(Vector3D(3,4,5).norm_l2() == 5*sqrt(2));
    assert(Vector3D(-3,4,5).norm_l2() == 5*sqrt(2));
    assert(Vector3D(3,-4,5).norm_l2() == 5*sqrt(2));
    assert(Vector3D(3,4,-5).norm_l2() == 5*sqrt(2));
    assert(Vector3D(0,0,0).norm_l2() == 0);

    // max
    assert(Vector3D(1,2,3).norm_inf() == 3);
    assert(Vector3D(1,6,3).norm_inf() == 6);
    assert(Vector3D(10,2,3).norm_inf() == 10);
    assert(Vector3D(-3,-2,-1).norm_inf() == -1);
    assert(Vector3D(1,3,3).norm_inf() == 3);
    assert(Vector3D(3,1,3).norm_inf() == 3);
    assert(Vector3D(3,3,1).norm_inf() == 3);
    assert(Vector3D(3,3,3).norm_inf() == 3);

    return SUCCESS;
}

int test_abs()
{
    for(int i = 0; i < 10; i++)
    {
        Vector3D a = abs(Vector3D(-1.0*i, i, -2.0*i));
        Vector3D b = Vector3D(i, i, 2.0*i);
        assert(a == b);
    }

    return SUCCESS;
}


int test_maxmin()
{
    assert(max(Vector3D(-1,  1,  1), 0)     == Vector3D(0, 1, 1));
    assert(max(Vector3D( 1, -1,  1), 0)     == Vector3D(1, 0, 1));
    assert(max(Vector3D( 1,  1, -1), 0)     == Vector3D(1, 1, 0));
    assert(max(Vector3D( 1, -1, -1), 0)     == Vector3D(1, 0, 0));
    assert(max(Vector3D(-1, -1, -1), 0)     == Vector3D(0, 0, 0));
    assert(max(Vector3D( 1,  1,  1), 0)     == Vector3D(1, 1, 1));
    assert(max(Vector3D( 1, -1, -1), 0.5)   == Vector3D(1, 0.5,0.5));


    assert(min(Vector3D(-1,  1,  1), 0)     == Vector3D(-1,  0,  0));
    assert(min(Vector3D( 1, -1,  1), 0)     == Vector3D( 0, -1,  0));
    assert(min(Vector3D( 1,  1, -1), 0)     == Vector3D( 0,  0, -1));
    assert(min(Vector3D( 1, -1, -1), 0)     == Vector3D( 0, -1, -1));
    assert(min(Vector3D(-1, -1, -1), 0)     == Vector3D(-1, -1, -1));
    assert(min(Vector3D( 1,  1,  1), 0)     == Vector3D( 0,  0,  0));
    assert(min(Vector3D( 1, -1, -1), 0.5)   == Vector3D( 0.5,-1,-1));


    vector<double> v = {1,-2,0.1};
    assert(minElement(v) == -2);


    return SUCCESS;
}


int test_operators()
{
    for(int i = -10; i < 10; i++)
    {
        assert(Vector3D(i,-i, i*i) == Vector3D(i, -i, i*i));    // comparison
        assert(Vector3D(i, 2*i, i*i) - Vector3D(i, 2*i, i*i) == Vector3D(0,0,0));
        assert(Vector3D(i, 2*i, i*i) + Vector3D(i, 2*i, i*i) == Vector3D(i+i,4*i,2*i*i));
        assert(Vector3D(-1,0,1)*i == Vector3D(-1*i, 0, i));
    }

    // Assignment operator
    Vector3D v;
    v = {1,2,3};
    assert(v == Vector3D(1,2,3));
    v = Vector3D(5,6,7);
    assert(v == Vector3D(5,6,7));

        
    return SUCCESS;
}

int test_constructor()
{
    // TODO: console output for other tests
    cout << " Testing constructors..." << endl;

    Vector3D v;
    v = {-1, 0, M_PI};
    assert(v.m_x == -1);
    assert(v.m_y == 0);
    assert(v.m_z == M_PI);


    cout << "OK!" << endl;
    return SUCCESS;
}




int main()
{
    assert(test_norms() == SUCCESS);
    assert(test_operators() == SUCCESS);
    assert(test_abs() == SUCCESS);
    assert(test_maxmin() == SUCCESS);
    assert(test_constructor() == SUCCESS);
    

    cout << "All Tests in test_vector3D passed!" << endl;
    return SUCCESS;
}