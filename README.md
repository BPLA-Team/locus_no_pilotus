# Locus No Pilotus: Trajectory Calculator

<div align="center">
  <img src=".extra/images/icon.png" alt="Logo" width="80" height="80">
</div>

**Project of four first grade MIPT DAFE/RSE students (for engineering practical work in the second semester) in Qt C++**

## Documentation

Project's [documentation](https://bpla-team.github.io/locus_no_pilotus/index.html) generated with Doxygen with [convenient CSS theme](#used-libs-and-sources).

# Contents

- [Description](#description)
- [Installation and configuring](#installation-and-configuring "with using MSYS")
- [Used libs and sources](#used-libs-and-sources "we are using GitHub submodules feature 😎")
- [Authors](#authors "the best guys")
- [Presentation](#presentation "video link")

## Description

In this project, we are developing an application that calculates the trajectory of a flying delivery robot that collects valuable cargo from control points: **Targets**.
On its way, it may encounter high mountains that it cannot fly over: **Hills**; or elliptical territories that are impossible to visit due to climate conditions: **TrappyCircles**.
Also, through some control points, the robot is simply unable to move, as the cargo may not be ready for transportation at that time, these are **TrappyLines**.

The robot's trajectory is calculated using several mathematical algorithms: [...].
The graphical interface for constructing the trajectory was created using [Qt](#used-libs-and-sources) and [QCustomPlot](#used-libs-and-sources).

In the application, you can add objects using window forms, interact with the trajectory calculation plot using the mouse cursor, create and open files in `.json` format with a specific style for this application. Editing objects can also be done with cursors or using a special dynamic input field with tables, opened in a separate window mode or embedded in the main one.

_We strongly recommend that you install our application using the instructions below and try it out!_

## Installation and configuring

#### On Windows 64 bit system

1. Install **[MSYS2](https://www.msys2.org/)** to any convenient folder as compiler setup

   > _P.S. of course you can try install all the libs and packages used in repo manually, but our team got pain and tears trying to install Boost on MINGW in this way, so we recommend to install MSYS (besides, it is an excellent tool for compiling any other C++ and etc. projects)_

2. Open `MSYS2 MSYS` console, copy the command below and paste it with `Shift+Ins` or `RBM and 'Paste'` to download all the packages used in our project

```
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-gdb
pacman -S mingw-w64-x86_64-ninja
pacman -S mingw-w64-x86_64-cmake
pacman -U https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-qt-creator-13.0.1-1-any.pkg.tar.zst
pacman -S mingw-w64-x86_64-boost
pacman -S mingw-w64-x86_64-clang
```

3. Install **[Git Bash](https://gitforwindows.org/)** to any convenient folder (if you haven't get it yet)

4. Open `Git Bash` and use command like `cd C:/CodeFolder` to go to the folder where you usually save the code

5. Clone our repo with including submodules to such folder:

```
git clone --recurse-submodules https://github.com/BPLA-Team/locus_no_pilotus
```

6. Open the MSYS bin folder in path like `C:\YourPathToMsys\msys64\mingw64\bin` than find and start `qtcreator.exe`

7. In folder with our project clone find **_CMakeLists.txt_** and open it with QtCreator

8. Set the compiler that allow _CMake_ configuration in the kits list and click `Configure Project`

9. To use the full working version: **Build** (hammer button in the lower left corner) the project, and after ending process with error use **_Build > Run CMake_** in the top menu to reconfigure and fix error with including QCustomPlot

   > _P.S. because of using QCustomPlot, we need to copy additional .lib file to build directory, and our script does this when activating the Run CMake command_

10. Now you can use full working project with **Run** (green triangle button in the lower left corner)!

#### On Linux system

_Coming soon..._

> Much respect and help for this installation method to [George Sukhanov](https://github.com/TheFueRr "our colleague with an equally interesting project on processing experimental data")!

## Used libs and sources

- [CMake](https://cmake.org/): main project build system
- [Qt](https://www.qt.io/): main project library for full-working program
- [QCustomPlot](https://www.qcustomplot.com/): library for drawing all objects on same place with autoscaling ([submodule](https://github.com/UmbrellaLeaf5/qcustomplot "reference for submodule with lib in GitHub"))
- [IceCream-Cpp](https://github.com/renatoGarcia/icecream-cpp): library for simple code debugging (we really recommend to use it in C++ projects)
- [Doxygen](https://www.doxygen.nl/): full documentation generation
- [Doxygen Awesome](https://github.com/jothepro/doxygen-awesome-css): convenient CSS theme for Doxygen HTML documentation (it is really awesome)
- [Flaticon](https://www.flaticon.com/): perfect icons source
- [GeoGebra](https://www.geogebra.org/): best platform for geometry calculations

## Authors

**[Romanov Fedor](https://github.com/Romanov-Fedor "math greatest gigachad and refactor guy (also Desmos and GeoGebra proger)")**

**[Rybalkin Ilya](https://github.com/Stargazer2005 "traveling salesman problem and Dijkstra algos enjoyer, the trajectory guy")**

**[Akramov Nikita](https://github.com/MrWh1teF0x "jsons, add forms, cursors, animation, scale hero")**

**[Krivoruchko Dmitry](https://github.com/UmbrellaLeaf5 "repo manager and gui guy with tables instead of muscles and arcs instead of veins")**
