#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Web.h"
#include <string>
#include <fstream>
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HW04obissiam_1App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);

private:
		Surface* mySurface_; //Our surface

	// For animation
	int frame_number_;
	boost::posix_time::ptime app_start_time_;

	// Dimensions of window
	static const int appWidth = 800;
	static const int appHeight = 460;
	static const int surfaceSize = 1024;

	void imageToSurface(uint8_t* surface, uint8_t* image);
	void circle(uint8_t* surface, int centerX, int centerY, int radius, Color8u fillColor);

};