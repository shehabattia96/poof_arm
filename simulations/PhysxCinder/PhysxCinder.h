#ifndef PhysxCinder_h
#define PhysxCinder_h
#include "cinder/params/Params.h"
#include "cinder/CameraUi.h"
#include "../BaseCinderApp/src/BaseCinderApp.h"

using namespace std;

/**
 * PhysxCinder handles displaying a list of SimulationObject(s) created in BaseCinderApp and returning a pointer to the current selectedObject.
*/
struct PhysxCinderStruct {
	public:
		PhysxCinderStruct() {};
		typedef struct PhysxCinderStruct type;

		vector<string> namesOfObjects;

		SimulationObject* selectedObject = NULL;

		int selectedObjectIndex = 0;

		void setSelectedObject(map<string, SimulationObject::type>* mSimulationObjectsMap) {
			if (namesOfObjects.size() > 0)
				this->selectedObject = &mSimulationObjectsMap->at(namesOfObjects[this->selectedObjectIndex]);
		}

		SimulationObject* getSelectedObject() {
            return this->selectedObject;
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

class PhysxCinder : public BaseCinderApp {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void updateGuiParams(bool updateNamesOfObjectsList);
  private:
	CameraUi mCameraUi;
	bool mPrintFps;
	params::InterfaceGlRef mParams;
	PhysxCinderStruct::type PhysxCinder;
};
#endif