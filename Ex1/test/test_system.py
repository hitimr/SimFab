import subprocess
import pathlib
import os
from filecmp import cmp


WORKSPACE_DIR = str(pathlib.Path(__file__).parent.parent) + "/"
OUT_DIR = WORKSPACE_DIR + "out/"
TEST_RES_DIR = WORKSPACE_DIR + "test/res/"
BINARY = OUT_DIR + "build/grid"


def test_rectangle():
    # invoke binary and compare output 
    cmd = str(BINARY + ' 4 3 0.1 Rectangle 1 1 3 2 ').split()
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    assert(cmp(OUT_DIR+"grid.vtk", TEST_RES_DIR + "grid_rect_middle.vtk", shallow=False))
    return 0



def test_rectangle():
    # invoke binary and compare output 
    cmd = str(BINARY + ' 4 4 0.1 Sphere 2 2 0 1 ').split()
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    assert(cmp(OUT_DIR+"grid.vtk", TEST_RES_DIR + "grid_sphere_middle.vtk", shallow=False))
    return 0




if __name__ == "__main__":
    assert(pathlib.Path(WORKSPACE_DIR).exists())
    assert(os.path.exists(BINARY))


    #test_rectangle()
    #test_rectangle()
    # todo: rework system tests