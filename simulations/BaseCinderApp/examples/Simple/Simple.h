#ifndef Simple_h
#define Simple_h
#include "cinder/params/Params.h"
#include "../../src/BaseCinderApp.h"

using namespace std;

class Simple : public BaseCinderApp {
  public:
	void setup() override;
	void draw() override;
};
#endif