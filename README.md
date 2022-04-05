![Vengine3D Logo](https://github.com/vadimvinokurov/vengine3D/blob/master/logo.png)
# vengine3D
Vengine3D is my first 3D physics engine to improve my programming skills and explore the field of game development.

## Build using CMake
- Install [CMake](https://cmake.org/)
- Ensure CMake is in the user `PATH`
```
git clone https://github.com/vadimvinokurov/vengine3D.git
cd vengine3D
git submodule init
git submodule update
mkdir build
cd build
cmake ..
cmake --build . --config Release
cp -R ..\contents .
```
- The "contents" folder must be located one level down in the filesystem than the executable file.

## Features
Physics engine implemented as:
- OpenGL with GLFW
- Implemented OOP Abstraction layer for OpenGL
- GUI base on ImGUI
- Collision system based on GJK algorithm and use convex shape and sphere, multiple shapes per body.
- Collision system give contact point and depth vector
- Physics engine use constraints physics (collision constraints solver with сontact, friction, and restitution, joint constraints)
- Physics engine stable stacking with a linear-time solver (base on warmstarting and accumulating)
- Load mesh and animation clips from glft file
- Play animation
