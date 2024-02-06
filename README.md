# OpenGL-Sample

Cat N' Mouse game built with C++ and OpenGL

## Building

### Building on Windows

Requires MinGW, GLFW and OpenGL.  The software also requires the GLM and glad libraries, but these header-only libraries are included in this repository.

1) Install MinGW, download from [here](https://sourceforge.net/projects/mingw/).  Ensure mingw32-gcc-g++ is selected in MinGW Installation Manager before proceeding.
2) Install GLFW and OpenGL.

### Building on Linux

Requires gcc, g++, pkg-config, GLFW and OpenGL.  The software also requires the GLM and glad libraries, but these header-only libraries are included in this repository.

Instructions are given with Debian-based package managers (apt)

1) Install gcc and g++ (if not already installed)
```shell
sudo apt-get install gcc g++
```
2) Install pkg-config (if not already installed)
```shell
sudo apt-get install pkg-config
```
3) Install OpenGL
```shell
sudo apt-get install libgl1-mesa-dri
```
4) Install GLFW
```shell
sudo apt-get install libglfw3 libglfw3-dev
```
5) Compile the code
```shell
cd OpenGL-Sample
make
```
6) Run the game
```shell
./sample
```

## Running the pre-built binaries

