#ifndef ObjectPicker_h
#define ObjectPicker_h
#include "../../src/BaseCinderApp.h"

using namespace std;

class ObjectPicker : public BaseCinderApp {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void updateSettingsSideBarParameters(bool updateNamesOfObjectsList);
  private:
	bool mPrintFps;
	params::InterfaceGlRef settingsSideBarParameters;
};
#endif