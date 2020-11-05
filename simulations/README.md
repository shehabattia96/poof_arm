# Physics simulations for the ball and socket arm

## What we are we simulating?

The simulation follows a micro-services architecture, serving an API to add objects or attach controllers.

The simulation GUI renders OBJ files and fits collision boxes and physics to each component of the ball and socket joint.

To create a first-approximation specification of which motors we'll need to use and where we will place our anchors on the robot arm, the simulation uses parametric modeling to dynamically change the joint configuration.

The philosophy behind designing this simulator is to de-couple the controllers, models and physics engine from each other.

## High level process

1. A Cinder application consumes arm components (Mesh/OBJ), renders and applies 3D transformations to them in a GUI.
2. Physics engine simulates gravity, joints definitions (revolute, prismatic, fixed or a combination in 3D space), collisions and (hopefully) the soft-body dynamics of the rope tugging on the arm.
3. All the `objects` types (actuators, sensors) will be controlled by an ESP32 and a sub-pub system, this will be standalone controller code that can be uploaded directly to the ESP32.
4. The sub-pub server and a central control application will control the robot and collect simulation data on optimal anchoring positions, collisions between arm components, angles where singularities occur, static/dynamic forces and torque.