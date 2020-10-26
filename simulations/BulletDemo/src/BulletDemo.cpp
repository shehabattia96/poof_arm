
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "btBulletDynamicsCommon.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class BulletDemo : public App
{
public:
   void setup() override;
   void mouseDown(MouseEvent event) override;
   void update() override;
   void draw() override;
   void setupPhysics();

   btAlignedObjectArray<btCollisionShape*> mCollisionShapes;
   btCollisionDispatcher* mDispatcher;
   btDefaultCollisionConfiguration* mCollisionConfiguration;


   btBroadphaseInterface *overlappingPairCache;
   btSequentialImpulseConstraintSolver* mSolver;
   btDiscreteDynamicsWorld *mDynamicsWorld;
   double mLastElapsedSeconds;

   CameraPersp mCam;
};

void BulletDemo::draw()
{ // this is called every frame per second per window
   gl::enableDepthRead();
   gl::enableDepthWrite();
   gl::enableAlphaBlending();
   gl::clear(Color::white());
   // gl::setViewport(getWindowBounds());
   gl::setMatrices(mCam);
   // drawGrid(100.0f, 10.0f);
   gl::color(Color::white());
   for (int j = mDynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
         btScalar m[17];
         trans.getOpenGLMatrix(m);
         gl::pushModelMatrix();
         glMultMatrixf((GLfloat *)m);
         
         gl::drawColorCube(vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ())),vec3(1));
         gl::popModelMatrix();
			printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}
}

void BulletDemo::setup()
{
	// Setup our default camera
	mCam.lookAt( vec3( 0, -5, -30 ), vec3( 0, 0.5f, 0 ) );
   setupPhysics();
   mLastElapsedSeconds = getElapsedSeconds();
}

void BulletDemo::mouseDown(MouseEvent event)
{
}

void BulletDemo::update()
{
   mDynamicsWorld->stepSimulation( getElapsedSeconds() - mLastElapsedSeconds );
   mLastElapsedSeconds = getElapsedSeconds();
}


void BulletDemo::setupPhysics()
{
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	mCollisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mSolver = new btSequentialImpulseConstraintSolver;

	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, overlappingPairCache, mSolver, mCollisionConfiguration);

	mDynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	///-----initialization_end-----


	///create a few basic rigid bodies

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		mCollisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		mDynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		// btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		mCollisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

      for (int i = 0; i < 30; i++){
		startTransform.setOrigin(btVector3(
         btScalar( randFloat(-5.f, 5.f) ),
         btScalar(2.0*i + 10),
         btScalar( randFloat(-5.f, 5.f) ) ));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		mDynamicsWorld->addRigidBody(body);
      }
	}
}

CINDER_APP(BulletDemo, RendererGl(RendererGl::Options().msaa(16)))