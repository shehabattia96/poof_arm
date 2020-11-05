#include "PhysxCinder.h"
#include "PhysxBase.h"

bool isInitPhysics = false;

void PhysxCinder::draw()
{ // this is called every frame per second per window
	gl::clear(Color(0.5, 0.5, 0.5));
	BaseCinderApp::draw();
	
	gl::setMatrices( mCam );

	// Draw the settings widget
	// mParams->draw();
	if (isInitPhysics) {
		stepPhysics();
		
		// PhysX draw actors
		physx::PxFlags<physx::PxActorTypeFlag::Enum, physx::PxU16> allActorTypes = PxActorTypeFlag::eRIGID_DYNAMIC|PxActorTypeFlag::eRIGID_STATIC;
		PxU32 numActors = gScene -> getNbActors(allActorTypes);
		console()<<"numActors "<<numActors<<std::endl;
		if (numActors) {
			vector<PxRigidActor*> actors(numActors);
			gScene->getActors(allActorTypes, (PxActor**)&actors[0], numActors);
			PxShape* shapes[128];
			for (PxU32 i = 0; i < numActors; i++)
			{
				const PxU32 nbShapes = actors[i]->getNbShapes();
				
				actors[i]->getShapes(shapes, nbShapes);
				// References https://github.com/kmiloarguello/openGL-physX/blob/master/1simpledrawing/Render.cpp
				// const bool sleeping = actors[i]->is<PxRigidDynamic>() ?
				// 					actors[i]->is<PxRigidDynamic>()->isSleeping() : false;

				for (PxU32 j = 0; j < nbShapes; j++)
				{
					const PxMat44 pose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
					const PxGeometry geometry = shapes[j]->getGeometry().any();

					glMatrixMode(GL_MODELVIEW);

					// render object
					glPushMatrix();
					glMultMatrixf(pose.front());
					
					console()<<"item "<<i<<" z position"<<pose.getPosition().z<<std::endl;
					
					gl::drawColorCube(vec3(pose.getPosition().x, pose.getPosition().y, pose.getPosition().z),vec3(2));
					glPopMatrix();
				}
			}
		}
	}
}

void PhysxCinder::setup()
{
	mCameraUi.setCamera( &mCam );
	initPhysics();
	isInitPhysics = true;
	/***** GUI Settings *****/
	// setFullScreen(true);
	setFrameRate(60);
	gl::enableVerticalSync();

	// enable Z-buffering to make shapes smoother around the edges https://libcinder.org/docs/guides/opengl/part2.html
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// Setup our default camera
	mCam.lookAt(vec3(40, 50, 70),vec3(0, 0, -25)); // lookAt(vec3 eyePoint, vec3 target). eyePoint controls rotation, target controls translation.

	/***** Create Settings Widget *****/

	// Create an PhysxCinderStruct to keep track of the selected object
	this->PhysxCinder = PhysxCinderStruct();
	// Generate Settings widget
	updateGuiParams(true);
}

/**
 *  updateGuiParams creates a Settings Widget if it doesn't exist and re-populates it with parameters.
*/
void PhysxCinder::updateGuiParams(bool updateNamesOfObjectsList)
{
	if (updateNamesOfObjectsList)
	{
		this->PhysxCinder.updateNamesOfObjectsList(BaseCinderApp::getSimulationObjectsMap());
	}
	mParams.reset();

	ivec2 paramWindowSize = ivec2(200, 400);
	mParams = params::InterfaceGl::create(getWindow(), "App parameters", toPixels(paramWindowSize));

	if (this->PhysxCinder.getSelectedObject() != NULL)
	{

		mParams->addParam(
			"Object Size",
			(function<void(ci::vec3)>)[this](ci::vec3 scale) { this->PhysxCinder.getSelectedObject()->scale = scale; },
			(function<ci::vec3()>)[this]()->ci::vec3 { return this->PhysxCinder.getSelectedObject()->scale; });
		mParams->addParam(
			"Object Rotation",
			(function<void(glm::highp_quat)>)[this](glm::highp_quat rotation) { this->PhysxCinder.getSelectedObject()->rotation = rotation; },
			(function<glm::highp_quat()>)[this]()->glm::highp_quat { return this->PhysxCinder.getSelectedObject()->rotation; });
		mParams->addParam(
			"Object Color",
			(function<void(ci::ColorA)>)[this](ci::ColorA color) { this->PhysxCinder.getSelectedObject()->color = color; },
			(function<ci::ColorA()>)[this]()->ci::ColorA { return this->PhysxCinder.getSelectedObject()->color; });

		mParams->addSeparator();

		mParams->addParam("Selected Object", PhysxCinder.namesOfObjects, &PhysxCinder.selectedObjectIndex)
			.keyDecr("[")
			.keyIncr("]")
			.updateFn([this] {
				PhysxCinder.setSelectedObject(BaseCinderApp::getSimulationObjectsMap());
			});
	}
	else
	{
		mParams->addText("Status Text", "label=`Please add an object and click refresh.`");
	}

	mParams->addSeparator();

	mParams->addParam("print fps", &mPrintFps).keyIncr("p");

	mParams->addButton("Refresh", [&]() { PhysxCinder::updateGuiParams(true); });
}

void PhysxCinder::update()
{
	if (mPrintFps && getElapsedFrames() % 60 == 0)
		console() << getAverageFps() << endl;
}

void PhysxCinder::mouseDown(MouseEvent event)
{
	mCameraUi.mouseDown( event );
}
void PhysxCinder::mouseDrag(MouseEvent event)
{
	mCameraUi.mouseDrag( event );
}

void prepareSettings(App::Settings *settings)
{
	settings->setHighDensityDisplayEnabled();
}
CINDER_APP(PhysxCinder, RendererGl(RendererGl::Options().msaa(16)), prepareSettings) // render with anti-aliassing
