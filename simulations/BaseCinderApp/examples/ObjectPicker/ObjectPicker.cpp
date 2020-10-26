#include "ObjectPicker.h"


void ObjectPicker::draw()
{ // this is called every frame per second per window
	gl::clear( Color( 0.5, 0.5, 0.5 ) );
	BaseCinderApp::draw();

	// Draw the settings widget
	mParams->draw();
}

void ObjectPicker::setup()
{
	
	/***** GUI Settings *****/
	// setFullScreen(true);
	setFrameRate(30);
	gl::enableVerticalSync();


	// enable Z-buffering to make shapes smoother around the edges https://libcinder.org/docs/guides/opengl/part2.html
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Setup our default camera
	mCam.lookAt( vec3( -15, -5, -2 ), vec3( 0, 0.5f, 0 ) );

	/***** Create Cubes *****/
	// generate stock color shader
	gl::GlslProgRef shader = gl::getStockShader( gl::ShaderDef().lambert().color() );

	// create a cube and add color
	geom::SourceMods slice = geom::Cube();
	
	// apply modifiers and initialize a new SimulationObject
	SimulationObject cube(slice, shader); // second argument defaults to the stock shader.
	
	// add the SimulationObject to a map so we can retreive it by name
	(*BaseCinderApp::getSimulationObjectsMap())["cube"] = cube;

	
	// apply modifiers and initialize a new SimulationObject
	SimulationObject cube2(slice >> geom::Translate(ci::vec3(0, 2, 0)), shader);
	Color redColor = Color( CM_RGB, 1, 0, 0 );
	cube2.color = redColor;
	
	// add the SimulationObject to a map so we can retreive it by name
	(*BaseCinderApp::getSimulationObjectsMap())["cube2"] = cube2;


	/***** Create Settings Widget *****/

	// Create an ObjectPickerStruct to keep track of the selected object
	this->objectPicker = ObjectPickerStruct();
	// Generate Settings widget
	updateGuiParams(true);
}

/**
 *  updateGuiParams creates a Settings Widget if it doesn't exist and re-populates it with parameters.
*/
void ObjectPicker::updateGuiParams(bool updateNamesOfObjectsList) {
	if (updateNamesOfObjectsList) {
		this->objectPicker.updateNamesOfObjectsList( BaseCinderApp::getSimulationObjectsMap() );
	}
	mParams.reset();


	ivec2 paramWindowSize = ivec2( 200, 400 );
	mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( paramWindowSize ) );


	if (this->objectPicker.getSelectedObject() != NULL) {
		
		mParams->addParam("Cube Size",
			(function<void( ci::vec3 )>) [this](ci::vec3 scale){this->objectPicker.getSelectedObject()->scale = scale;},
			(function<ci::vec3 ()>)[this]() -> ci::vec3 {return this->objectPicker.getSelectedObject()->scale;}
		);
		mParams->addParam( "Cube Rotation",
			(function<void( glm::highp_quat )>) [this](glm::highp_quat rotation){this->objectPicker.getSelectedObject()->rotation = rotation;},
			(function<glm::highp_quat ()>)[this]() -> glm::highp_quat {return this->objectPicker.getSelectedObject()->rotation;}
		);
		mParams->addParam( "Cube Color",
			(function<void( ci::ColorA )>) [this](ci::ColorA color){this->objectPicker.getSelectedObject()->color = color;},
			(function<ci::ColorA ()>)[this]() -> ci::ColorA {return this->objectPicker.getSelectedObject()->color;}
		);
		
		mParams->addSeparator();

		mParams->addParam( "Selected Object", objectPicker.namesOfObjects, &objectPicker.selectedObjectIndex )
			.keyDecr( "[" )
			.keyIncr( "]" )
			.updateFn( [this] { 
				objectPicker.setSelectedObject( BaseCinderApp::getSimulationObjectsMap() );
			} );
			
	} else {
		mParams->addText( "Status Text", "label=`Please add an object and click refresh.`");
	}

	mParams->addSeparator();

	mParams->addParam( "print fps", &mPrintFps ).keyIncr( "p" );

	mParams->addButton( "Refresh",[&]() { ObjectPicker::updateGuiParams(true); });
}

void ObjectPicker::update()
{
	if( mPrintFps && getElapsedFrames() % 60 == 0 )
		console() << getAverageFps() << endl;
}

void ObjectPicker::mouseDown( MouseEvent event )
{
	// grab cube
	SimulationObject* cube = &(*BaseCinderApp::getSimulationObjectsMap())["cube"];
	float modifier = (event.isRightDown() ? -1.f : 1.f) * .15f;
	// apply a transformation, which will get drawn and reset in the next frame
	cube->translation = ci::vec3(modifier*10 + cube->translation.x, 0, 0);
	cube->rotation = angleAxis( modifier + cube->rotation.w, vec3( cube->rotation.x + modifier, cube->rotation.y + modifier, cube->rotation.z +modifier));

	
	// grab cube
	SimulationObject* cube2 = &(*BaseCinderApp::getSimulationObjectsMap())["cube2"];
	// apply a transformation, which will get drawn and reset in the next frame
	cube2->translation = ci::vec3(-1 * modifier*10 + cube2->translation.x, 0, 0);
	cube2->rotation = angleAxis(-1 * modifier + cube2->rotation.w, vec3( cube2->rotation.x -1 * modifier, cube2->rotation.y-1 * modifier, cube2->rotation.z-1 * modifier));
}

void prepareSettings( App::Settings *settings )
{
	settings->setHighDensityDisplayEnabled();
}
CINDER_APP( ObjectPicker, RendererGl(RendererGl::Options().msaa(16)), prepareSettings ) // render with anti-aliassing
