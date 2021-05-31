# ViennaLS Example

This repo contains a minimal example to show how to install ViennaHRLE and ViennaLS on a bare system and then include them as dependencies in an external project.


```
mkdir ViennaTools && cd ViennaTools
export VIENNATOOLS_DIR=`pwd`

# Installing ViennaHRLE
cd $VIENNATOOLS_DIR
git clone https://github.com/ViennaTools/viennahrle.git
cd viennahrle
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install/
make install


# Installing ViennaLS
cd $VIENNATOOLS_DIR
git clone https://github.com/ViennaTools/viennals.git
cd viennals
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install/
make install


# Building the example
cd $VIENNATOOLS_DIR
git clone https://github.com/ViennaTools/viennals-example.git
cd viennals-example
mkdir build && cd build
cmake .. -DViennaHRLE_DIR=../viennahrle/install/ -DViennaLS_DIR=../viennals/install/
make
```

After the example has been built, you can run the program by:

```
./Frosty
```

This will create a level set snowman and advect it to simulate it melting.
