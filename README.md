# MrGeo
Bringing together the FourierBlender &amp; CircularNURBS

[![Example Video](https://img.youtube.com/vi/a-JByk379Ig/0.jpg)](https://www.youtube.com/watch?v=a-JByk379Ig)

## Build & Install Guide (For now Ubuntu only)
This is a hacky prototype, no guarantees! To make it run in Blender 2.8 follow these steps.

0. Install Eigen3 so that CMake can find it, e.g.
```console
$ sudo apt install libeigen3-dev
```
1. Download the code
```console
$ git clone https://github.com/Vollkornaffe/MrGeo/
```
2. Create a build dir
```console
$ cd MrGeo && mkdir build && cd build
```
3. Run CMake (providing an install dir, e.g. ~/install)
```console
$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/install ..
```
4. Build & Install
```console
$ make install
```
5. Run Blender 2.8 & open the ~/install/MrGeo.py
  
The script generates a "Control" object which is used to control the circular NURBS surface.
One needs to comment out the "init_poly_surface" call in the script before re-running the script, otherwise the changes in the mesh are overriden.
