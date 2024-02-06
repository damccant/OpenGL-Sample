# Cat N' Mouse Game

Cat N' Mouse game built with C++ and OpenGL

## Building

### Building on Windows

Requires MinGW, GLFW and OpenGL.  The software also requires the GLM and glad libraries, but these header-only libraries are included in this repository.

1) Install MinGW, download from [here](https://sourceforge.net/projects/mingw/).  Ensure mingw32-gcc-g++ is selected in MinGW Installation Manager before proceeding.
2) Install GLFW and OpenGL.
3) Run MinGW make
```shell
cd cat-n-mouse
mingw32-make
```
4) Run the game
```shell
./catnmouse.exe
```

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
cd cat-n-mouse
make
```
6) Run the game
```shell
./catnmouse
```

## Running the pre-built binaries

The pre-built binaries have been compiled statically, meaning that no extra libraries are required to run them.

### Windows

Run the provided `.exe` file.  If prompted, click Run.

### Linux

Run the provided executable.  This executable is linked against `libc`, so ensure that it is installed.