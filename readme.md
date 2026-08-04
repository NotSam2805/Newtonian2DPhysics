# Newtonian Physics Simulation
A complete, self contained, Physics Engine and Render Pipeline. All components should be kept as simple as possible (and limited to 2D).

# Structure
Standard Physics Engine pipeline as follows:
Apply Forces -> Detect Collisions -> Resolve Collisons -> Integrate Motion

The Renderer will run completely independantly, simply accessing the necessary positions from the Physics Engine to render the objects.
