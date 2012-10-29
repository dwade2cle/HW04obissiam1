#include "Web.h"
#include <string>
#include <fstream>
#include "Resources.h"
#include "cinder\DataSource.h"
#include "stdafx.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HW04obissiam_1App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void HW04obissiam_1App::setup()
{
	//FILE* f;
	//const char* gFile = "Starbucks_2006.csv";
	//string pathToFile = getAssetPath(fs::path(gFile)).string();
	//if (fs::exists(pathToFile));
	//f = fopen(pathToFile.c_str(), "r");
	Entry myEntries[7655];
	string line; double x, y;
	int count = 0;
	ifstream myfile;
	//loadResource(RES_MY_RES, "DATA");
	//loadResource(RES_MY_RES);
	//myfile.open("C:\\Users\\AO\\Documents\\CSE 274\\HW04obissiam_1\\resources\\Starbucks_2006.csv", ifstream::in);
	myfile.open("../resources/Starbucks_2006.csv", ifstream::in);
	if (myfile.is_open())	{
		while (!myfile.eof())	{
			Entry* entry = new Entry();
			getline(myfile, line, ',');
			entry->identifier = line;
			myfile >> x;
			entry->x = x;
			while(myfile.get() != ',' && !myfile.eof());
			myfile >> y;
			myEntries[count].y = y;
			entry->y = y;
			//myEntries.push_back(entry);
			myEntries[count] = *entry;
			count++;
			while(myfile.get() != '\r' && !myfile.eof()); 
		}
		myfile.close();
	} else cout << "Unable to open file" << endl;
	Node* root = new Node();
	Web* web = new Web(root);
	web->build(myEntries, count);
	Entry* e = web->getNearest(0.95, 0.5);
	cout << e->identifier << ' ' << e->x << ' ' << e->y << endl; 
}

void HW04obissiam_1App::mouseDown( MouseEvent event )
{
}

void HW04obissiam_1App::update()
{
}

void HW04obissiam_1App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( HW04obissiam_1App, RendererGl )
