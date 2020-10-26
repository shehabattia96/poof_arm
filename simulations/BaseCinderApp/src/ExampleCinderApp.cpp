#include "cinder/params/Params.h"
#include "BaseCinderApp.cpp"


void prepareSettings( App::Settings *settings )
{
	settings->setHighDensityDisplayEnabled();
}



struct ObjectPicker {
	public:
		ObjectPicker() {};
		typedef struct ObjectPicker type;

		vector<string> namesOfObjects;

		SimulationObject* selectedObject = NULL;

		int selectedObjectIndex = 0;

		void setSelectedObject(map<string, SimulationObject::type>* mSimulationObjectsMap) {
			if (namesOfObjects.size() > 0)
				this->selectedObject = &mSimulationObjectsMap->at(namesOfObjects[this->selectedObjectIndex]);
		}

		void updateNamesOfObjectsList(map<string, SimulationObject::type>* mSimulationObjectsMap) {
			this->namesOfObjects.clear();
			for ( const auto &mSimulationObjectsKeyValuePair : *mSimulationObjectsMap )
			{
				string objectName = mSimulationObjectsKeyValuePair.first;
				this->namesOfObjects.push_back(objectName);
			}
			setSelectedObject(mSimulationObjectsMap);
		}
};

class ExampleCinderApp : public BaseCinderApp {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void resize();
	void updateGuiParams(bool updateObjectPicker);
  private:
	bool mPrintFps;
	params::InterfaceGlRef mParams;
	ObjectPicker::type objectPicker;
};

void ExampleCinderApp::setup()
{
	
	// setFullScreen(true);
	setFrameRate(30);
	gl::enableVerticalSync();


	// enable Z-buffering to make shapes smoother around the edges https://libcinder.org/docs/guides/opengl/part2.html
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// generate colorful shaders
	gl::GlslProgRef shader = gl::getStockShader( gl::ShaderDef().lambert().color() );

	// create a cube and add color
	geom::SourceMods slice = geom::Cube();
	
	// apply modifiers and initialize a new SimulationObject
	SimulationObject cube(slice, shader); // second argument defaults to the stock shader.
	
	// add the SimulationObject to a map so we can retreive it by name
	mSimulationObjectsMap["cube"] = cube;

	
	// apply modifiers and initialize a new SimulationObject
	SimulationObject cube2(slice >> geom::Translate(ci::vec3(0, 2, 0)), shader);
	Color redColor = Color( CM_RGB, 1, 0, 0 );
	cube2.color = redColor;
	
	// add the SimulationObject to a map so we can retreive it by name
	mSimulationObjectsMap["cube2"] = cube2;

	// Setup our default camera
	mCam.lookAt( vec3( -15, -5, -2 ), vec3( 0, 0.5f, 0 ) );

	// Create the interface and give it a name.
	objectPicker = ObjectPicker();
	updateGuiParams(true);
}

void ExampleCinderApp::updateGuiParams(bool updateObjectPicker) {
	if (updateObjectPicker) {
		objectPicker.updateNamesOfObjectsList(&mSimulationObjectsMap);
	}
	mParams.reset();
	mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( ivec2( 200, 400 ) ) );
	SimulationObject* selectedObject = objectPicker.selectedObject;
	if (selectedObject != NULL) {
		ci::ColorA* color = &(selectedObject->color);
		
		glm::highp_quat rotation = angleAxis( 0.f, vec3( 0, 0, 0 ) );
		ci::vec3 translation = ci::vec3(0, 0, 0);
		ci::vec3 scale = ci::vec3(1, 1, 1);
		mParams->addParam( "Cube Size", &(selectedObject->scale) ).min( 0.1f ).max( 20.5f ).keyIncr( "z" ).keyDecr( "Z" ).precision( 2 ).step( 0.02f );

		mParams->addParam( "Cube Rotation", &(selectedObject->rotation) );
		mParams->addParam( "Cube Color", color);
		
		mParams->addSeparator();

		mParams->addParam( "Selected Object", objectPicker.namesOfObjects, &objectPicker.selectedObjectIndex )
			.keyDecr( "[" )
			.keyIncr( "]" )
			.updateFn( [this] { objectPicker.setSelectedObject(&mSimulationObjectsMap); ExampleCinderApp::updateGuiParams(false); } );
	} else {
		mParams->addText( "Status Text", "label=`Please add an object and click refresh.`");
	}

	mParams->addSeparator();

	mParams->addParam( "print fps", &mPrintFps ).keyIncr( "p" );

	mParams->addButton( "Refresh",[&]() { ExampleCinderApp::updateGuiParams(true); });
}

void ExampleCinderApp::resize()
{
	mCam.setAspectRatio( getWindowAspectRatio() );
}

void ExampleCinderApp::update()
{
	if( mPrintFps && getElapsedFrames() % 60 == 0 )
		console() << getAverageFps() << endl;
}

void ExampleCinderApp::mouseDown( MouseEvent event )
{
	// grab cube
	SimulationObject* cube = &mSimulationObjectsMap["cube"];
	float modifier = (event.isRightDown() ? -1.f : 1.f) * .15f;
	// apply a transformation, which will get drawn and reset in the next frame
	cube->translation = ci::vec3(modifier*10 + cube->translation.x, 0, 0);
	cube->rotation = angleAxis( modifier + cube->rotation.w, vec3( cube->rotation.x + modifier, cube->rotation.y + modifier, cube->rotation.z +modifier));

	
	// grab cube
	SimulationObject* cube2 = &mSimulationObjectsMap["cube2"];
	// apply a transformation, which will get drawn and reset in the next frame
	cube2->translation = ci::vec3(-1 * modifier*10 + cube2->translation.x, 0, 0);
	cube2->rotation = angleAxis(-1 * modifier + cube2->rotation.w, vec3( cube2->rotation.x -1 * modifier, cube2->rotation.y-1 * modifier, cube2->rotation.z-1 * modifier));
}

void ExampleCinderApp::draw()
{ // this is called every frame per second per window
	gl::clear( Color( 0.5, 0.5, 0.5 ) );
	BaseCinderApp::draw();

	// Draw the interface
	mParams->draw();
}

CINDER_APP( ExampleCinderApp, RendererGl(RendererGl::Options().msaa(16)), prepareSettings ) // render with anti-aliassing
