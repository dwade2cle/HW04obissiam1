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
	void keyDown( KeyEvent event );
	void update();
	void draw();
	void prepareSettings(Settings* settings);

private:
	// Dimensions of window
	static const int appWidth = 750;
	static const int appHeight = 412;
	static const int surfaceSize = 1024;

	Surface* mySurface_; //Our surface
	uint8_t dummySurface[3*surfaceSize*surfaceSize];
	uint8_t* dataArray;
	Web* web;
	Node* root;
	Vec2i mousePosition;
	bool leftClicked;
	Entry* currentStarbucks;
	double magicNumber;
	Node* currentNode;
	bool topRight;

	// For animation
	int frame_number_;
	boost::posix_time::ptime app_start_time_;

	// Loads an image onto our surface
	// Starts to fulfill A
	void imageToSurface(uint8_t* surface, uint8_t* image);
	// Shows how close each pixel is to a starbucks location
	// Fulfills C in a different way
	void proximityMap();
	// Draws a circle at a starbucks location
	void circle(uint8_t* surface, int centerX, int centerY, int radius, Color8u fillColor);
	// Zooms to one corner of the map
	// Fulfills half of D
	void drawMap(uint8_t* picture, Node* root, bool zoom, int x, int y, double magicNumber);
	// Draws a map based on the census information
	// Fulfills F
	void censusMap(uint8_t* picture);

};