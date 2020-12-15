#ifndef PhysxBase_h
#define PhysxBase_h

#include <ctype.h>
#include "PxConfig.h"
#include "PxPhysicsAPI.h"
using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;
PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;
PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene		= NULL;
PxMaterial*				gMaterial	= NULL;
PxPvd*                  gPvd        = NULL;
PxReal stackZ = 10.0f;

PxRigidDynamic* createProjectile(const PxTransform& transform, const PxGeometry& geometry, const PxVec3& velocity=PxVec3(0)) {	
	PxRigidDynamic* actor = PxCreateDynamic(*gPhysics, transform, geometry, *gMaterial, 200.0f);
	actor->setAngularDamping(0.5f);
	actor->setLinearVelocity(velocity);
	gScene->addActor(*actor);
	return actor;
}

void PhysxCinder::createSimulationObject(string objectId, PxRigidActor* actor, geom::SourceMods shape, gl::GlslProgRef shader, ci::ColorA* color)
{
	SimulationObject simulationObject(shape, shader);
	if (color != NULL) simulationObject.color = *color;
	SimulationObject* cubeRef = &simulationObject;

	simulationObject.preDrawFunction = [this, actor,objectId] () {
			PxMat44 pose(actor->getGlobalPose());
			mat4 mat(pose.column0.x, pose.column0.y, pose.column0.z, pose.column0.w, 
								pose.column1.x, pose.column1.y, pose.column1.z, pose.column1.w,
								pose.column2.x, pose.column2.y, pose.column2.z, pose.column2.w,
								pose.column3.x, pose.column3.y, pose.column3.z, pose.column3.w);

			SimulationObject* simulationObject = &(*BaseCinderApp::getSimulationObjectsMap())[objectId];
			simulationObject->worldPose = mat;
	};
	// add the SimulationObject to a map so we can retreive it by name
	(*BaseCinderApp::getSimulationObjectsMap())[objectId] = simulationObject;
}


std::vector<PxRigidDynamic*> createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent/2, halfExtent/2, halfExtent/2), *gMaterial);
	std::vector<PxRigidDynamic*> actorsArray;
	for(PxU32 i=0; i<size;i++)
	{
		for(PxU32 j=0;j<size-i;j++)
		{
			PxTransform localTm(PxVec3(PxReal(j*2) - PxReal(size-i), PxReal(i*2+1), 0) * halfExtent / 2);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);

			actorsArray.push_back(body);
		}
	}
	shape->release();
	return actorsArray;
}

void PhysxCinder::initPhysics()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	PxTolerancesScale tolerance = PxTolerancesScale();
	tolerance.length = 1;
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, tolerance,true,gPvd);

	PxSceneDesc sceneDesc(tolerance);
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if(pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxPlane groundPlaneShape = PxPlane(0,1,0,0);
	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, groundPlaneShape, *gMaterial);
	gScene->addActor(*groundPlane);
	geom::SourceMods geomGroundPlaneShape = geom::Cube() >> geom::Scale(glm::vec3(0.1, 100, 100));
	ci::ColorA whiteColor = ci::ColorA( CM_RGB, 1, 1, 1, 1 );
	createSimulationObject("ground", groundPlane, geomGroundPlaneShape, NULL, &whiteColor);

	int numCubes = 1;
	float cubeSize = 2.0f;
	for(PxU32 i=0;i<5;i++)
		for(PxRigidActor* actor : createStack(PxTransform(PxVec3(0,0,stackZ-=10.0f)), 10, cubeSize)) {
			createSimulationObject("cube"+std::to_string(numCubes), actor, geom::Cube().size(glm::vec3(cubeSize)), NULL, NULL);
			numCubes++;
		}

	ci::ColorA redColor = ci::ColorA( CM_RGB, 1, 0, 0, 1 );
	float radius = 0.5;
	createSimulationObject("projectile", 
		createProjectile(PxTransform(PxVec3(0,10,50)),PxSphereGeometry(radius),PxVec3(-2,-3,-50)), 
		geom::Sphere().radius(radius), NULL, &redColor);
	createSimulationObject("projectile2", 
		createProjectile(PxTransform(PxVec3(0,15,50)),PxSphereGeometry(radius),PxVec3(-2,-3,-50)), 
		geom::Sphere().radius(radius), NULL, &redColor);
	radius = 5;
	createSimulationObject("projectile3", 
		createProjectile(PxTransform(PxVec3(10,15,50)),PxSphereGeometry(radius),PxVec3(-2,-3,-50)), 
		geom::Sphere().radius(radius), NULL, &redColor);
}

void stepPhysics()
{
	gScene->simulate(1.0f/60.0f);
	gScene->fetchResults(true);
}


// TODO:cleanupPhysics
void cleanupPhysics()
{
}



#endif