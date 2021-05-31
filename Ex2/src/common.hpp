#pragma once
#include <string>
#include <array>
#include <iostream>
#include <vector>
#include <assert.h>

#include <lsVTKWriter.hpp>
#include <lsSmartPointer.hpp>
#include <lsDomain.hpp>
#include <lsVelocityField.hpp>

using NumericType = double;
constexpr int D = 3;


//#define MICROMETERS *0.000001
#define MICROMETERS *1

// taken from https://dev.to/aggsol/calling-shell-commands-from-c-8ej
std::string execCommand(const std::string cmd, int& out_exitStatus)
{
    out_exitStatus = 0;
    auto pPipe = ::popen(cmd.c_str(), "r");
    if(pPipe == nullptr)
    {
        throw std::runtime_error("Cannot open pipe");
    }

    std::array<char, 256> buffer;

    std::string result;

    while(not std::feof(pPipe))
    {
        auto bytes = std::fread(buffer.data(), 1, buffer.size(), pPipe);
        result.append(buffer.data(), bytes);
    }

    auto rc = ::pclose(pPipe);

    if(WIFEXITED(rc))
    {
        out_exitStatus = WEXITSTATUS(rc);
    }

    return result;
}


int renderMesh(lsSmartPointer<lsMesh<NumericType> > & mesh, std::string fileName, bool displayImage=true)
{
    std::cout << "Rendering " << fileName << std::endl;
    std::string vtkFile = "out/vtk/" + fileName + ".vtk";
    std::string pngFile = "out/images/" + fileName + ".png";
    lsVTKWriter<NumericType>(mesh, vtkFile).apply();


    int retCode = -69;
    std::string command = "python3 src/renderVtk.py " +  vtkFile + " " + pngFile;


    if(displayImage == true)
    {
        command += " --display";
    }
    else
    {
        command += " --no-display";
    }

    std::cout << "invoking: " << command << std::endl;
    execCommand(command, retCode);

    if(retCode == 0)
    {
        std::cout << "Render sucessful" << std::endl;
    }
    else
    {
        std::cout << "oopsie whoopsie I did a fucky wucky QwQ" << std::endl;
    }

    return retCode;
}



// from https://viennatools.github.io/ViennaLS/doxygen/html/SquareEtch_8cpp-example.html#a31
class ConstantVelocityField : public lsVelocityField<double>
{
private:
    double m_velocity;

public:
    ConstantVelocityField(double velocity = -1.0) : lsVelocityField() 
    {
        m_velocity = velocity; 
    }
    double getScalarVelocity(const std::array<double, 3> & /*coordinate*/,
                            int material,
                            const std::array<double, 3> &normalVector,
                            unsigned long /*pointId*/) {

        return m_velocity;
    }
}; 


class ConstantDirectionalVelocityField : public lsVelocityField<double>
{
private:
    double m_velocity;
    std::vector<double> m_direction;

public:
    ConstantDirectionalVelocityField(double velocity, std::vector<double> direction) : lsVelocityField() 
    {
        assert(direction.size() == 3);

        m_direction.resize(3);
        m_direction[0] = direction[2];
        m_direction[1] = direction[1];
        m_direction[2] = direction[0];

        
        m_direction = direction;
    }
    double getScalarVelocity(const std::array<double, 3> & /*coordinate*/,
                            int material,
                            const std::array<double, 3> &normalVector,
                            unsigned long /*pointId*/) {

        return m_velocity * (
            normalVector[0] * m_direction[0] +
            normalVector[1] * m_direction[1] +
            normalVector[2] * m_direction[2]);
    }
}; 



