# Rendering

Uses SDL2 to render a [PhysicsWorld](#physicsworld) to the screen.

## Camera

[Camera.hpp](Examples/Rendering/Camera.hpp)

Struct that holds a [Vector2](#vector2) position, and a float zoom. The position is the world position of the camera, screen positions are calculated from this position. Zoom determines the scale of the screen relative to the world, a zoom of 10 means that 1 world unit is 10 screen units.

## Renderer

[Renderer.hpp](Examples/Rendering//Renderer.hpp)
[Rednerer.cpp](Examples/Rendering/Renderer.cpp)

### Renderer Properties

| Property | Type | Description |
| -------- | ---- | ----------- |
| window | SDL_Window* | The window to draw to |
| renderer | SDL_Renderer* | The SDL renderer that actually handles rendering points and triangles |
| backgroundColour | [Colour](#colour) | The colour for the background |

### Constuctor

| Parameter | Type | Default |
| --------- | ---- | ------- |
| windowWidth | int | none |
| windowHeight | int | none |
| camera | [Camera](#camera) | {[Vector2](#vector2)::Zero(), 1.0} |
| backgroundColour | [Colour](#colour) | {33, 33, 33, 255} |

### DrawBody

Determines the [Shape](#shape) of the body, and calls one of: [DrawCircle](#drawcircle), [DrawPolygon](#drawpolygon), *[DrawRect](#drawrect)*.

### DrawCircle

Draws a [Circle](#circle) to the screen.

### DrawPolygon

Draws a [Polygon](#polygon) to the screen. Find pairs of vertexes -> Draw edge -> Push vertex to vertices -> Use vertices to find triangles -> Draw triaangles.

### *DrawRect*

**DEPRECATED**

Draws a [Rect](#rect) to the screen.