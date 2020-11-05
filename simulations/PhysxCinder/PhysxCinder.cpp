#include "PhysxCinder.h"
#include "PhysxBase.h"

bool isInitPhysics = false;

void PhysxCinder::draw()
{ // this is called every frame per second per window
	gl::clear(Color(0.5, 0.5, 0.5));
	
	gl::setMatrices( mCam );

	// Draw the settings widget
	settingsSideBarParameters->draw();
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
	BaseCinderApp::draw();
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

	// Create an SettingsSideBarStruct to keep track of the selected object
	this->settingsSideBar = SettingsSideBarStruct();
	// Generate Settings widget
	updateSettingsSideBarParameters(true);
}

/**
 *  updateSettingsSideBarParameters creates a Settings Widget if it doesn't exist and re-populates it with parameters.
*/
void PhysxCinder::updateSettingsSideBarParameters(bool updateNamesOfObjectsList)
{
	if (updateNamesOfObjectsList)
	{
		this->settingsSideBar.updateNamesOfObjectsList(BaseCinderApp::getSimulationObjectsMap());
	}
	settingsSideBarParameters.reset();

	ivec2 paramWindowSize = ivec2(200, 400);
	settingsSideBarParameters = params::InterfaceGl::create(getWindow(), "App parameters", toPixels(paramWindowSize));

	if (this->settingsSideBar.getSelectedObject() != NULL)
	{

		settingsSideBarParameters->addParam(
			"Object Size",
			(function<void(ci::vec3)>)[this](ci::vec3 scale) { this->settingsSideBar.getSelectedObject()->scale = scale; },
			(function<ci::vec3()>)[this]()->ci::vec3 { return this->settingsSideBar.getSelectedObject()->scale; });
		settingsSideBarParameters->addParam(
			"Object Rotation",
			(function<void(glm::highp_quat)>)[this](glm::highp_quat rotation) { this->settingsSideBar.getSelectedObject()->rotation = rotation; },
			(function<glm::highp_quat()>)[this]()->glm::highp_quat { return this->settingsSideBar.getSelectedObject()->rotation; });
		settingsSideBarParameters->addParam(
			"Object Color",
			(function<void(ci::ColorA)>)[this](ci::ColorA color) { this->settingsSideBar.getSelectedObject()->color = color; },
			(function<ci::ColorA()>)[this]()->ci::ColorA { return this->settingsSideBar.getSelectedObject()->color; });

		settingsSideBarParameters->addSeparator();

		settingsSideBarParameters->addParam("Selected Object", this->settingsSideBar.namesOfObjects, &this->settingsSideBar.selectedObjectIndex)
			.keyDecr("[")
			.keyIncr("]")
			.updateFn([this]() {
				this->settingsSideBar.setSelectedObject(BaseCinderApp::getSimulationObjectsMap());
			});
	}
	else
	{
		settingsSideBarParameters->addText("Status Text", "label=`Please add an object and click refresh.`");
	}

	settingsSideBarParameters->addSeparator();

	settingsSideBarParameters->addParam("print fps", &mPrintFps).keyIncr("p");

	settingsSideBarParameters->addButton("Refresh", [&]() { PhysxCinder::updateSettingsSideBarParameters(true); });
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

CINDER_APP(PhysxCinder, RendererGl(RendererGl::Options().msaa(16)), prepareSettingsSideBar) // render with anti-aliassing
