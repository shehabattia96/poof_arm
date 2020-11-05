#ifndef BaseCinderApp_h
#define BaseCinderApp_h

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Shader.h"


using namespace ci;
using namespace ci::app;


/**
 * SimulationObject stores a geom::Source, generates the gl::BatchRef and handles the transformations and drawing of a geom::Source.
 * 
 * Example use:
geom::SourceMods slice = geom::Cube();
SimulationObject cube(slice >> geom::Translate(ci::vec3(0, 2, 0)), shader);
mSimulationObjectsMap["cube"] = cube;

* Example cube transformation:
SimulationObject* cube = &mSimulationObjectsMap["cube"];
cube->translation = ci::vec3(2 + cube->translation.x, 0, 0);
cube->rotation = angleAxis(.5f + cube->rotation.w, vec3( 0, 0, 0 ) );
cube->scale = ci::vec3(1 + cube->scale.x, 0, 0);

* Example change color:
Color redColor = Color( CM_RGB, 1, 0, 0 );
cube2->color = redColor;
 */
struct SimulationObject {

	private: // we perform additional logic when these are modified
		geom::SourceMods source; // e.g. geom::SourceMods source = geom::Cube();
		gl::GlslProgRef shader;
		gl::BatchRef batchRef;

	public:
		SimulationObject() {}
		
		SimulationObject(geom::SourceMods sourceIn) : source(sourceIn) {
			this->shader = gl::getStockShader( gl::ShaderDef().lambert().color() );
			rebuildBatchRef();
		}
		SimulationObject(geom::SourceMods sourceIn, gl::GlslProgRef shaderIn) : source(sourceIn), shader(shaderIn) {
			rebuildBatchRef();
		}

		void rebuildBatchRef() {
			this->batchRef = gl::Batch::create( this->source, this->shader );
		}

		gl::BatchRef getBatchRef() {
			return this->batchRef;
		}

		geom::SourceMods getSource() {
			return this->source;
		}

		void setSourceAndRebuildBatchRef(geom::SourceMods source) {
			this->source = source;
			rebuildBatchRef();
		}
		
		void setShaderAndRebuildBatchRef(gl::GlslProgRef shader) {
			this->shader = shader;
			rebuildBatchRef();
		}
		
		typedef struct SimulationObject type;

		ci::ColorA color = ci::ColorA( CM_RGB, 1, 0.5, 0.25, 1 );
		
		glm::highp_quat rotation = angleAxis( 0.f, vec3( 0, 0, 0 ) );
		ci::vec3 translation = ci::vec3(0, 0, 0);
		ci::vec3 scale = ci::vec3(1, 1, 1);

		float* matrix;

		void draw() {
			gl::translate(this->translation);
			gl::rotate(this->rotation);
			gl::scale(this->scale);
			gl::color(this->color);
			glMultMatrixf(matrix);


			this->batchRef->draw();
		}

};

class BaseCinderApp : public App {
	public:
		void setup() override;
		void update() override;
		void draw() override;
		void resize();
		std::map<std::string, SimulationObject::type>* getSimulationObjectsMap();
	
	protected:
		CameraPersp	mCam;
		std::map<std::string, SimulationObject::type> mSimulationObjectsMap;
};

std::map<std::string, SimulationObject::type>* BaseCinderApp::getSimulationObjectsMap() {
	return &(this->mSimulationObjectsMap);
}

void BaseCinderApp::draw()
{ // this is called every frame per second per window
	
	// gl::clear( Color( 0.5, 0.5, 0.5 ) ); // this is required
	gl::setMatrices( mCam );
	
	for ( const auto &mSimulationObjectsKeyValuePair : mSimulationObjectsMap )
	{
		// pushes and pops model, replaces gl::pushModelMatrix(); and gl::popModelMatrix(); 
		gl::ScopedModelMatrix scpModelMtx;

		// grab the SimulationObject and call its draw metod
		// string objectName = mSimulationObjectsKeyValuePair.first;
		SimulationObject object = mSimulationObjectsKeyValuePair.second;
		object.draw();
	}
}

void BaseCinderApp::setup()
{
	// mCam.lookAt( vec3( -15, -5, -2 ), vec3( 0, 0.5f, 0 ) );
}

void BaseCinderApp::resize()
{
	this->mCam.setAspectRatio( getWindowAspectRatio() ); // helps keep the drawn objects' scale when the GUI window is resized.
}

void BaseCinderApp::update()
{}

#endif