# Documentation

## Contents

| Heading         |
|-----------------|
| [Maths](#maths) |
| [Physics](#physics) |
| [Rendering](#rendering) |
| [Shapes](#shapes) |

## Maths

Essentially acts as a library to provide much of the mathematical functions and structures needed to the rest of the engine.

### Constants

[Maths](/Maths)

All mathematical constants are defined here.

PI is set to 3.14159265358979323846, this value is more than accurate for any calculations from the engine.

DEG_TO_RAD is used to convert angles, eg: degrees = radians \* DEG_TO_RAD

RAD_TO_DEG is used to convert angles, eg: radians = degrees \* RAD_TO_DEG

### MathsUtils

[MathsUtils.hpp](/Maths/MathsUtils.hpp)
[MathsUtils.cpp](/Maths/MathsUtils.cpp)

Some utility functions. I dont think I actually used them in the engine, these functions are also included in the \<cmath\> header

### Vector2

[Vector2.hpp](/Maths/Vector2.hpp)
[Vector2.cpp](/Maths/Vector2.cpp)

Most of the physics is based on calculations using 2D Vectors. All functions needed are included here.

#### Constructor

Takes X and Y component, defaults to (0,0).

#### Zero

Static function, returns a new [Vector2](#vector2), calls default constructor.

#### Magnitude

Returns the magnitude of a [Vector2](#vector2). Use should be avoided where possible, in favour of [MagnitudeSqrd](#magnitudesqrd).

#### MagnitudeSqrd

Returns the square of the magnitude of a [Vector2](#vector2). This is useful because it does not use a sqrt function, saving time.

#### Normalised

Returns a new [Vector2](#vector2), one unit in the same direction.

#### Normalise

Sets the [Vector2](#vector2) to one unit in the same direction.

#### Dot

Returns the dot product of the [Vector2](#vector2).

#### Cross

Returns the cross product.

#### Operators

Provide mathematical operations for [Vector2](#vector2).

### Transform

[Transform.hpp](/Maths/Transform.hpp)

Holds position and rotation. Can be used to transform [Vector2](#vector2) into different spaces.

## Physics

[Physics](/Physics)

The state of a simulation is held here, this handles the operations of each physics step, including the movements of all bodies in the world.

### Rigidbody

[Rigidbody.hpp](/Physics/Rigidbody.hpp)
[Rigidbody.cpp](/Physics/Rigidbody.cpp)

A solid body, can be affected by physics, does not deform.

#### Rigidbody Properties

| Property | Type | Description |
| -------- | ---- | ----------- |
| mass | float | Mass of the body |
| inverseMass | float | So the inverse of the mass only is calculated once |
| transform | [Transform](#transform) | Holds the current position and rotation of the body |
| velocity | [Vector2](#vector2) | The current velocity of the body, in units/s |
| accumulatedForces | [Vector2](#vector2) | The sum of all forces added to the body, to be applied at the next velocity [Integrate](#integrate) |
| rotationalVelocity | float | The current rotational velocity of the body |
| accumulatedTorque | float | The sum of all torques added to the body, to be applied at the next rotational velocity [Integrate](#integrate) |
| inertia | float | The moment of inertia of the body |
| inverseInertia | float | So the inverse of the inertia only is calculated once |
| shape | std::unique_ptr<[Shape](#shape)> | The shape of the body |
| restitution | float | Value from 0-1 which determines "bounciness" |
| friction | float | Value from 0-1 which determines "slippiness" |

#### Constructor

| Parameter | Default |
| ----------- | --------- |
| mass | 1.0 |
| transform | [Transform()](#transform) |
| shape | [Circle](#circle)(1.0, [Colour](#colour)(255, 255, 255, 255)) |
| inertia | 1.0 |

#### AddForce

Adds a force to the body, to be applied at next [Integrate](#integrate).

#### AddTorque

Adds a torque to the body, defined as radians clockwise, to be applied at the next [Integrate](#integrate).

#### Integrate

Uses semi-implicit Euler. Applies all forces to change velocity, updates position from velocity. Applies all torques, updates rotational velocity, updates rotation.

#### IntegratePosition

Uses semi-implict Euler. Updates position and rotation based on velocities.

#### IntegrateVelocity

Uses semi-implict Euler. Applies forces and updates velocity. Applies torques and updates rotational velocity.

#### ClearForces

Resets accumulated forces.

#### ClearTorques

Resets accumulated torques.

#### Move

Changes the position of the body by the displacement.

#### ApplyImpulse

Applies an impulse to the body.

Overload takes a point, the impulse is applied from this point and the torque of this impulse is applied.

#### GetVelocitAtPoint

Finds the velocity of the body at a given point, accounting for rotational velocity.

#### Getters

| Function | Returns |
| ---------- | --------- |
| GetPosition() | [Vector2&](#vector2) |
| GetVelocity() | [Vector2&](#vector2) |
| GetRotationalVelocity() | float |
| GetRotation() | float |
| GetMass() | float |
| GetInverseMass() | float |
| GetInverseInertia() | float |
| GetShape() | [Shape*](#shape) |
| GetTransform() | [Transform&](#transform) |

### PhysicsWorld

[PhysicsWorld.hpp](/Physics/PhysicsWorld.hpp)
[PhysicsWorld.cpp](/Physics/PhysicsWorld.cpp)

The main class for applying all physics and dealing with interactions.

#### Physics World Properties

| Property | Type | Description |
| -------- | ---- | ----------- |
| bodies | std::vector<[Rigidbody*](#rigidbody)> | holds all rigidbodies in the world |
| gravity | [Vector2](#vector2) | The force of gravity, applied every step to all objects |

#### Step

Performs one time step, moving dt seconds.

Apply forces -> Integrate Velocity -> Collision Detection -> Collision Solving -> Integrate Position. Loop through all bodies.

#### AddBody

Add a reference to a [Rigidbody](#rigidbody) to the world.

## Rendering

Uses SDL2 to render a [PhysicsWorld](#physicsworld) to the screen.

### Camera

[Camera.hpp](/Rendering/Camera.hpp)

Struct that holds a [Vector2](#vector2) position, and a float zoom. The position is the world position of the camera, screen positions are calculated from this position. Zoom determines the scale of the screen relative to the world, a zoom of 10 means that 1 world unit is 10 screen units.

### Renderer

[Renderer.hpp](/Rendering/Renderer.hpp)
[Rednerer.cpp](/Rendering/Renderer.cpp)

#### Renderer Properties

| Property | Type | Description |
| -------- | ---- | ----------- |
| window | SDL_Window* | The window to draw to |
| renderer | SDL_Renderer* | The SDL renderer that actually handles rendering points and triangles |
| backgroundColour | [Colour](#colour) | The colour for the background |

#### Constuctor

| Parameter | Type | Default |
| --------- | ---- | ------- |
| windowWidth | int | none |
| windowHeight | int | none |
| camera | [Camera](#camera) | {[Vector2](#vector2)::Zero(), 1.0} |
| backgroundColour | [Colour](#colour) | {33, 33, 33, 255} |

#### DrawBody

Determines the [Shape](#shape) of the body, and calls one of: [DrawCircle](#drawcircle), [DrawPolygon](#drawpolygon), *[DrawRect](#drawrect)*.

#### DrawCircle

Draws a [Circle](#circle) to the screen.

#### DrawPolygon

Draws a [Polygon](#polygon) to the screen. Find pairs of vertexes -> Draw edge -> Push vertex to vertices -> Use vertices to find triangles -> Draw triaangles.

#### *DrawRect*

**DEPRECATED**

Draws a [Rect](#rect) to the screen.

## Shapes

Handles geometery, used for collisions in simulations, and for rendering bodies.

### Shape

[Shape.hpp](/Shapes/Shape.hpp)

### Circle

[Circle.hpp](/Shapes/Circle.hpp)
[Circle.cpp](/Shapes/Circle.cpp)

Inherits from [Shape](#shape).

### Polygon

[Polygon.hpp](/Shapes/Polygon.hpp)
[Polygon.cpp](/Shapes/Polygon.cpp)

Inherits from [Shape](#shape). Uses a std::vector<[Vector2*](#vector2)> to hold vertices of the shape.

#### GetWorldVertex

Get the vertex in world space (Transformed by the given [Transform](#transform)).

### Colour

[Colour.hpp](/Shapes/Colour.hpp)

Struct to hold RGB values.

### *Rect*

**DEPRECATED**

Inherits from [Shape](#shape). A rectangle of width x height. Deprecated due to the constraint of being axis-aligned, use [Polygon](#polygon).

## Collision