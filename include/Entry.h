//#include "cinder/gl/gl.h"
//#include "cinder/app/AppBasic.h"
//#include "stdafx.h"
#include <string>
#include <iostream>
//using namespace ci;
//using namespace ci::app;

using namespace std;

class Entry
{
	public:
		int x_;
		int y_;
		string locationName_;
		
		// Empty entry
		Entry();
		
		Entry(string name, int x, int y);
		
		int getX();
		
		int getY();
		
		string getName();
};