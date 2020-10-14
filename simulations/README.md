# Bullet simulations for ball and socket arm

## Setup

`BULLET_PHYSICS_SOURCE_DIR` path variable needs to be set to wherever [https://github.com/bulletphysics/bullet3.git](https://github.com/bulletphysics/bullet3.git) is cloned.

## Ropes

### Example attached rope code

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