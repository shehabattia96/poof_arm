# Bullet simulations for ball and socket arm

## What we are we simulating?

The simulation follows a micro-services architecture, serving an API to add objects or attach controllers.

The simulation GUI renders OBJ files and fits Bullet3 collision boxes and physics to each component of the ball and socket joint.

To create a first-approximation specification of which motors we'll need to use and where we will place our anchors on the robot arm, the simulation uses parametric modeling to dynamically change the joint configuration.

The philosophy behind designing this simulator is to de-couple the controllers, models and physics engine from each other.

## High level process

1. A Cinder application consumes arm components (Mesh/OBJ), renders and applies 3D transformations to them in a GUI.
2. Bullet3 physics engine simulates gravity, joints definitions (revolute, prismatic, fixed or a combination in 3D space), collisions and (hopefully) the soft-body dynamics of the rope tugging on the arm.
3. All the `objects` types (actuators, sensors) will be controlled by an ESP32 and a sub-pub system, this will be standalone controller code that can be uploaded directly to the ESP32.
4. The sub-pub server and a central control application will control the robot and collect simulation data on optimal anchoring positions, collisions between arm components, angles where singularities occur, static/dynamic forces and torque.

## Setup

The following path variables should be set for your user:

- `BULLET_PHYSICS_SOURCE_DIR` path variable needs to be set to wherever [https://github.com/bulletphysics/bullet3.git](https://github.com/bulletphysics/bullet3.git) is cloned.
- `CINDER_PATH` path variable needs to be set to wherever [https://github.com/cinder/Cinder.git](https://github.com/cinder/Cinder.git) is cloned.

## References

### Ropes

#### Example attached rope code

[source](https://github.com/bulletphysics/bullet3/blob/master/examples/SoftDemo/SoftDemo.cpp#L504)
```
	//
	// Rope attach
	//
	static void Init_RopeAttach(SoftDemo* pdemo)
	{
		//TRACEDEMO
		pdemo->m_softBodyWorldInfo.m_sparsesdf.RemoveReferences(0);
		struct Functors
		{
			static btSoftBody* CtorRope(SoftDemo* pdemo, const btVector3& p)
			{
				btSoftBody* psb = btSoftBodyHelpers::CreateRope(pdemo->m_softBodyWorldInfo, p, p + btVector3(10, 0, 0), 8, 1);
				psb->setTotalMass(50);
				pdemo->getSoftDynamicsWorld()->addSoftBody(psb);
				return (psb);
			}
		};
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(12, 8, 0));
		btRigidBody* body = pdemo->createRigidBody(50, startTransform, new btBoxShape(btVector3(2, 6, 2)));
		btSoftBody* psb0 = Functors::CtorRope(pdemo, btVector3(0, 8, -1));
		btSoftBody* psb1 = Functors::CtorRope(pdemo, btVector3(0, 8, +1));
		psb0->appendAnchor(psb0->m_nodes.size() - 1, body);
		psb1->appendAnchor(psb1->m_nodes.size() - 1, body);
	}
```