#include "Simple.h"

void Simple::draw()
{ // this is called every frame per second per window
	gl::clear( Color( 0.5, 0.5, 0.5 ) );
	BaseCinderApp::draw();
}

/**
 * In setup, we will draw two cubes with different colors and initial transformations. 
*/
void Simple::setup()
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

	// create a cube
	geom::SourceMods slice = geom::Cube();
	
	// apply modifiers and initialize a new SimulationObject
	SimulationObject cube(slice, shader); // second argument defaults to the stock shader.
	
	// add the SimulationObject to a map so we can retreive it by name
	(*BaseCinderApp::getSimulationObjectsMap())["cube"] = cube;

	
	// apply modifiers and initialize a new SimulationObject for cube2
	SimulationObject cube2(slice >> geom::Translate(ci::vec3(0, 2, 0)), shader);
	Color redColor = Color( CM_RGB, 1, 0, 0 );
	cube2.color = redColor;
	
	// add the SimulationObject to a map so we can retreive it by name
	(*BaseCinderApp::getSimulationObjectsMap())["cube2"] = cube2;
}

CINDER_APP( Simple, RendererGl(RendererGl::Options().msaa(16)) ) // render with anti-aliassing
