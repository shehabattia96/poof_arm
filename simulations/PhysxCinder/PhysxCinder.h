#ifndef PhysxCinder_h
#define PhysxCinder_h
#include "cinder/CameraUi.h"
#include <ctype.h>
#include "PxConfig.h"
#include "PxPhysicsAPI.h"
#include "../BaseCinderApp/src/BaseCinderApp.h"

using namespace std;
using namespace physx;

class PhysxCinder : public BaseCinderApp {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void updateSettingsSideBarParameters(bool updateNamesOfObjectsList);
	void createSimulationObject(string objectId, PxRigidActor* actor, geom::SourceMods shape, gl::GlslProgRef shader, ci::ColorA* color);
	void initPhysics();
  private:
	CameraUi mCameraUi;
	bool mPrintFps;
	params::InterfaceGlRef settingsSideBarParameters;
};
#endif