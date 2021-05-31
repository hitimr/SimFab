#include <string>
#include <fstream>

class vtkOutput {
public:
  // the name of the file to generate
  std::string fileName;
  // the grid spacing so the data can be displayed in absolute coordinates
  // rather than integer indices
  const double delta;
  // a reference to the grid which stores all signed distance values
  std::vector<std::vector<double>> &data;




  vtkOutput(std::string file,
            double gridDelta,
            std::vector<std::vector<double>> &grid) :
              fileName(file), delta(gridDelta), data(grid) {}

  void write() {
    std::ofstream f(fileName.c_str());

    // Write the file header
    f << "# vtk DataFile Version 2.0" << std::endl;
    f << "2D Surface" << std::endl;
    f << "ASCII" << std::endl;
    f << "DATASET RECTILINEAR_GRID" << std::endl;
    // give the size in each cartesian direction
    // here a rectangular grid is assumed
    f << "DIMENSIONS " << data.size() << " " << data[0].size() <<
          " 1" << std::endl;

    // Write the first set of coordinates
    f << "X_COORDINATES " << data.size() << " float" << std::endl;
    for(unsigned i = 0; i < data.size(); ++i) {
      f << i * delta << std::endl;
    }

    // Write the second set of coordinates
    f << "Y_COORDINATES " << data.size() << " float" << std::endl;
    for(unsigned i = 0; i < data.size(); ++i) {
      f << i * delta << std::endl;
    }

    // Write the z = 0 coordinate, since vtk only allows for 3D
    f << "Z_COORDINATES 1 float" << std::endl;
    f << "0" << std::endl;

    // now print the data we want to display
    f << "POINT_DATA " << (data.size() * data.size()) << std::endl;
    // when displayed, the values will be shown as "SDValues"
    f << "SCALARS SDValues float" << std::endl;
    f << "LOOKUP_TABLE default" << std::endl;
    for(unsigned j = 0; j < data[0].size(); ++j) {
      for(unsigned i = 0; i < data.size(); ++i) {
        f << data[i][j] << " ";
      }
      f << std::endl;
    }
  }
};
