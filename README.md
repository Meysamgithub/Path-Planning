# Socket CAN Bus

C++ package for Socket CAN Bus (for test on Nvidia AGX Xavier board).

## Table of Contents
- [Dependencies](#dependencies)
    - [CMake](#cmake)
- [Running example applications](#Running-example-applications)
- [Other documentation](#other-documentation)

## Dependencies

Install the standard C/C++ development tools for your platform, as well as the following:

#### CMake

CMake version 3.14 or higher is required. 
```
sudo apt remove cmake # remove old version
wget https://cmake.org/files/v3.14/cmake-3.14.0.tar.gz
tar xf cmake-3.14.0.tar.gz 
cd cmake-3.14.0
./configure
sudo make install
cmake --version
```

## Running example applications

Use the following commands to build and run the standalone executable examples.

```bash
# Configure the build
cmake -S standalone -B build/standalone

# Build
cmake --build build/standalone -j4

# Run
./build/standalone/canbus_example
```

## Other documentation

- The [development.md](documentation/pages/development.md) contains instructions about the development of the project.