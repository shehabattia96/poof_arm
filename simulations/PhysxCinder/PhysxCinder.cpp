#include "PhysxCinder.h"
#include "PhysxBase.h"
#include "Sidebar.cpp"

bool isInitPhysics = false;

void PhysxCinder::draw()
{ // this is called every frame per second per window
	gl::clear(Color(0.5, 0.5, 0.5));

	// Draw the settings widget
	settingsSideBarParameters->draw();

	// interact with physics engine
	if (isInitPhysics) stepPhysics();

	// draw scene
	BaseCinderApp::draw(true);
}

void PhysxCinder::setup()
{
	mCameraUi.setCamera( &mCam );
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

	
	/***** Load models *****/
	initPhysics();
	isInitPhysics = true;
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
