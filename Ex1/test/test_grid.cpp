#include <assert.h>
#include "grid.h"

using namespace std;


int test_parse_border_types()
{
    Grid grid;

    vector<int> a = {BORDER_PERIODIC, BORDER_PERIODIC, BORDER_PERIODIC, BORDER_PERIODIC};
    char argv1[] = "pppp";
    assert(grid.parse_border_types(argv1) == a);


    vector<int> b = {BORDER_PERIODIC, BORDER_REFLECTIVE, BORDER_REFLECTIVE, BORDER_PERIODIC};
    char argv2[] = "prrp";
    assert(grid.parse_border_types(argv2) == b);
    assert(grid.parse_border_types(argv2) != a);


    vector<int> c = {BORDER_REFLECTIVE, BORDER_REFLECTIVE, BORDER_REFLECTIVE, BORDER_REFLECTIVE};
    char argv3[] = "rrrr";
    assert(grid.parse_border_types(argv3) == c);

    vector<int> d = {BORDER_REFLECTIVE, BORDER_PERIODIC, BORDER_PERIODIC, BORDER_REFLECTIVE};
    char argv4[] = "rppr";
    assert(grid.parse_border_types(argv4) == d);

    return SUCCESS;
}


int main()
{
    cout << "Testing grid..." << endl;

    assert(test_parse_border_types() == SUCCESS);

    cout << "All Tests in test_sdf passed!" << endl;
    return SUCCESS;
}