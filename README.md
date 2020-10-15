# Ball and Socket arm

## Our goal is to develop

An inexpensive (<$500) cable-actuated 5-dof collaborative robot arm with a max-reach of 30cm and a payload of 5kg.

The idea is to simply the design enough to eschew 3D-printed parts as much as possible, and use off the shelf items for as many components as possible.

Inspired by the human body and robots like AMBIDEX and Blue.

## Roadmap

Simulations will help approximate the best places for the cable to anchor, we'll look out for angles at which the joint experiences singularities or becomes too inefficient to operate and calcualte the reduction/mechanical advantage we need to achieve our goal.

- Simulate the ball and socket arm
  - Prepare a Cinder application to consume arm components (Mesh/OBJ), render and apply 3D transformations to them in a GUI. [x]
  - Integrate Bullet3 physics engine with the Cinder gui to simulate gravity, joints definitions (revolute, prismatic, fixed or a combination in 3D space), collisions and (hopefully) the soft-body dynamics of the rope tugging on the arm.
  - CAD/Blender all the components and export them as OBJ/MLF:
    [] ball and socket
    [] ball housing with anchoring holes for the string, mounting bores to the base of the arm, and mounting bores for the pulley housing.
    [] socket housing with mounting bores for the pulley housing and the rest of the arm.
    [] pulley housing to fit the 3mm pulleys and fix to the socket/ball housings.
    [] motor housing with mounting holes for the drum shaft and tensioning spring
    [] tensioning spring mechanism
    [] drum shaft mechanism
    [] Motor and motor coupling
  - Make a consumable robot descriptor that's better that URDF and SDF, preferably in a flat JSON structure and perhaps pre-compiled into the application for performance boosts. It should define `objects` as structural, connecting(screw/bolt/nut), mechanical (gear/pulley/belt), actuator, sensor, PCB, power source; `links` as solid 3D shapes or OBJ's with initial parameters: scale, rotation, soft or rigid material and color/texture; `joints` as revolute, prismatic, fixed or a combination in 3D space; `anchors` as XYZ locations on the respective link meshes to initially attach the links. The benefit here is the anchors, which makes it easy to assemble and mate the components of the robot.
  - Build a pipeline to dynamically load the arm component OBJ models into Cinder and Bullet3
  - Build software controllers for all the `objects` types (actuators, sensors) that will be controlled by an ESP32 and a sub-pub system.
  - Build a sub-pub server and a central control application, which will collect simulation data on optimal anchoring positions, collisions between arm components, angles where singularities occur, static/dynamic forces and torque. This information will be used to deduce the nominal torque we'll need from our motors.
  - Simulate the ball and socket joint.
  - CAD/Blender the 1DoF elbow joint and figure out how to extend cabling to the base of the arm.
  - Add the elbow model to the simulation.
  - Duplicate the ball and socket for the wrist joint and figure out how to extend cabling to the base of the arm.
  - Add a gripper.
  - Add additional sensors such as cameras and motor current/temp sensors.
  - Simulate the entire arm.
- Build the ball and socket arm
- Mount the arm to a moving platform

## Prototyping

Initial prototypes with aluminum sheet metal and worm-gear motors seem promising.

### Materials

- [1mm Steel string](https://www.amazon.com/gp/product/B07TX57Q9X/)
- [3mm v-groove pulleys with ball bearings](https://www.amazon.com/gp/product/B079GSTHSX/)
- Aluminum sheet metal (Lowes/Home Depot)
- [22mm Steel alloy ball and socket](https://www.amazon.com/gp/product/B06XKW7V14/) - this tripod head has a good ball and socket.