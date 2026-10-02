# Newtonian Physics Engine

A complete, self contained, Physics Engine and Render Pipeline. This is meant to provide the foundation for future projects (game engine ect), and be able to handle high numbers of objects efficiently. Complete simple render pipeline, physics handling, collision detection and resolution, and maths tools for all these systems.

Documentation can be found [here](doc.md)

Demo example is in [demo.cpp](Examples/demo.cpp) and is compiled in [demo.exe](build-ucrt64/N2Pdemo.exe)

![Recording of demo](n2pDemo.gif)

## Use

See the [template](Examples/template.cpp) for simple use for running the physics engine and renderer at ~60fps.


## Features

Currently is a 'bare-bones' system.

Rigidbody simulation, including:
- Position and velocity integration
- Force handling
- Torque and rotation handling
- Collision detection
  + AABB broad phase
  + SAT narrow phase
- Collision solving
  + Sequential impulse solver for multiple contacts
- Static bodies (Rigidbodies with mass, inertia, and their inverses set to 0)

## Planned features

- Constraints
- Tunneling prevention and handling
- 'Warm Starting' collision impulses
- Stabilisation (Baumgarte?)
- Debug drawing
- Physics Layers
- Multithreading optimisation
