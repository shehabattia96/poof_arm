#ifndef PhysxCinder_h
#define PhysxCinder_h
#include "cinder/CameraUi.h"
#include "../BaseCinderApp/src/BaseCinderApp.h"

using namespace std;

class PhysxCinder : public BaseCinderApp {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void updateSettingsSideBarParameters(bool updateNamesOfObjectsList);
  private:
	CameraUi mCameraUi;
	bool mPrintFps;
	params::InterfaceGlRef settingsSideBarParameters;
};
#endif