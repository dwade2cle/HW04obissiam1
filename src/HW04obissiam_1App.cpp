#include "HW04obissiam_1App.h"

void HW04obissiam_1App::setup()
{
	// Loading US map to mySurface_
	mySurface_ = new Surface(surfaceSize,surfaceSize,false);
	Surface US_picture(loadImage(loadResource(RES_US_PIC)));
	uint8_t* pic_data = US_picture.getData();	
	dataArray = (*mySurface_).getData();
	imageToSurface(dataArray, pic_data);

	static uint8_t picture[3*surfaceSize*surfaceSize];
	memcpy(picture, dataArray, 3*surfaceSize*surfaceSize);
	// Placing all entries in the web
	Entry myEntries[7655];
	string line; double x, y;
	int count = 0; 
	ifstream myfile;
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
			myEntries[count] = *entry;
			count++;
			while(myfile.get() != '\r' && !myfile.eof()); 
		}
		myfile.close();
	} else cout << "Unable to open file" << endl;
	
	root = new Node();
	web = new Web(root);
	web->build(myEntries, count);


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Program won't work properly with key events. So here are some examples of the different functions
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1.) Map of all locations with proximity of each pixel to nearest starbucks indicated
	// Note* If clicked, a dot will show which starbucks is closest
	proximityMap();
	static uint8_t picture1[3*surfaceSize*surfaceSize];
	memcpy(picture1, dataArray, 3*surfaceSize*surfaceSize);
	// Here we plot all of the locations
	vector<Entry> locations;
	web->getAllLocations(root, &locations);
	for(int i = 0; i < locations.size(); i++)	{
		double x = locations[i].x; 
		double y = locations[i].y;
		circle(dataArray, x*appWidth, (1-y)*appHeight, 2, Color8u(255, 215, 0));
	}
	locations.clear();
	// 2.) Zoom to a corner of the picture. Can only zoom in once.
	// Set current node for zoom function
	currentNode = root;

	// The zoom function will work once, but doesn't work with a key event for some reason
	/////////////////////////////////////////////////////////////////////////////////////
	// Uncommenting this line will zoom to the desired corner. Note, must keep proximityMap() for this to work.
	// top right corner
	//drawMap(picture1, currentNode->right_->right_, true, appWidth/2, 0, 0.025);
	// top left corner
	//drawMap(picture1, currentNode->left_->right_, true, 0, 0, 0.025);
	// bottom left corner
	//drawMap(picture1, currentNode->left_->left_, true, 0, appHeight/2, 0.025);
	// bottom right corner
	//drawMap(picture1, currentNode->right_->left_, true, appWidth/2, appHeight/2, 0.025);

	// 3.) Census Map, which shows areas where there are lots of starbucks per person and places
	// where there are not so many starbucks per person
	//censusMap(picture);

	// Set leftClicked for MouseDown method
	leftClicked = false;
	topRight = false;
}

void HW04obissiam_1App::prepareSettings(Settings* settings)	{
	(*settings).setWindowSize(appWidth, appHeight);
	(*settings).setResizable(false);
}

void HW04obissiam_1App::imageToSurface(uint8_t* surface, uint8_t* image)	{
	int offset, offset_image;
	for (int j = 0; j < appHeight; j++)	{
		for(int i = 0; i < appWidth; i++)	{
			offset = 3*(i + j*surfaceSize); 
			offset_image = 3*(i + j*appWidth);
			surface[offset + 0] = image[offset_image + 0];
			surface[offset + 1] = image[offset_image + 1];
			surface[offset + 2] = image[offset_image + 2];
		}
	}
}

void HW04obissiam_1App::circle(uint8_t* surface, int centerX, int centerY, int radius, Color8u fillColor)	{
	// Square around circle
	if (centerY-radius < 0 || centerX-radius < 0) return;
	for (int j = centerY - radius; j < (centerY + radius); j++)	{
		for (int i = centerX - radius; i < (centerX + radius); i++)	{
			// If inside circle radius, change pixel value
			int distX = std::abs(i - centerX);
			int distY = std::abs(j - centerY);
			if (std::sqrt((double)(distX*distX + distY*distY)) < radius)	{
				surface[3*(j*surfaceSize + i) + 0] = fillColor.r;
				surface[3*(j*surfaceSize + i) + 1] = fillColor.g;
				surface[3*(j*surfaceSize + i) + 2] = fillColor.b;
			}
		}
	}
}

void HW04obissiam_1App::proximityMap()	{
	// Setting pixels to indicate how close we are to a Starbucks based on getNearest method
	int offset;
	Entry* e;
	for (int j = 0; j < appHeight; j++)	{
		for(int i = 0; i < appWidth; i++)	{
			offset = 3*(i + j*surfaceSize);
			if(dataArray[offset + 2] == 255 && dataArray[offset] != 255)	{
				double x = (double)i/appWidth; double y = 1-(double)j/appHeight;
				int r,g,b; g = 0;
				e = web->getNearest(x, y);
				double distance = sqrt((x-e->x)*(x-e->x) + (y-e->y)*(y-e->y));
				if (distance > 0.05) (b = 255, r = 0);
				else	{
					b = distance*(1/0.05)*255;
					r = (0.05-distance)*(1/0.05)*255;
				}
				dataArray[offset + 0] = r;
				dataArray[offset + 1] = g;
				dataArray[offset + 2] = b;
			}
		}
	}
}
void HW04obissiam_1App::drawMap(uint8_t* picture, Node* root, bool zoom, int x, int y, double magicNumber)	{
	int offset, offsetZoom; int countI; int countJ = 0;
	uint8_t info[3*appHeight*appWidth/4];
	for(int j = 0; j < appHeight/2; j++)	{
		for(int i = 0; i < appWidth/2; i++)	{
			offset = 3*(i + j*appWidth/2);
			offsetZoom = 3*(x + i + (y + j)*surfaceSize);
			info[offset + 0] = picture[offsetZoom + 0];
			info[offset + 1] = picture[offsetZoom + 1];
			info[offset + 2] = picture[offsetZoom + 2];
		}
	}
	if (zoom)	{
		for (int j = 0; j < appHeight; j+=2)	{
			countI = 0;
			for(int i = 0; i < appWidth; i+=2)	{
				offset = 3*(i + j*surfaceSize); 
				offsetZoom = 3*(countI + countJ*appWidth/2);
				int r = dataArray[offsetZoom + 0];
				int g = dataArray[offsetZoom + 1];
				int b = dataArray[offsetZoom + 2];
				dataArray[offset + 0] = info[offsetZoom + 0];
				dataArray[offset + 1] = info[offsetZoom + 1];
				dataArray[offset + 2] = info[offsetZoom + 2];
				dataArray[offset + 3] = info[offsetZoom + 0];
				dataArray[offset + 4] = info[offsetZoom + 1];
				dataArray[offset + 5] = info[offsetZoom + 2];
				dataArray[3*surfaceSize + offset + 0] = info[offsetZoom + 0];
				dataArray[3*surfaceSize + offset + 1] = info[offsetZoom + 1];
				dataArray[3*surfaceSize + offset + 2] = info[offsetZoom + 2];
				dataArray[3*surfaceSize + offset + 3] = info[offsetZoom + 0];
				dataArray[3*surfaceSize + offset + 4] = info[offsetZoom + 1];
				dataArray[3*surfaceSize + offset + 5] = info[offsetZoom + 2];
				countI++;
			}
			countJ++;
		}
	}
	// Setting pixels to indicate how close we are to a Starbucks based on getNearest method
	/**
	Entry* e;
	double x1, y1;
	for (int j = 0; j < appHeight; j++)	{
		for(int i = 0; i < appWidth; i++)	{
			offset = 3*(i + j*surfaceSize);
			if(!(dataArray[offset + 0] == 0 && dataArray[offset + 1] == 0  && dataArray[offset + 1] == 0) && 
				!(dataArray[offset + 0] == 255 && dataArray[offset + 1] == 255 && dataArray[offset + 1] == 255))	{
				x1 = (double)(i/2+x)/appWidth; y1 = 1-((double)(j/2+y)/appHeight);
				int r,g,b; g = 0;
				e = web->getNearest(x1, y1);
				double distance = sqrt((x1-e->x)*(x1-e->x) + (y1-e->y)*(y1-e->y));
				if (distance > magicNumber) {b = 255; r = 0;}
				else	{
					b = distance*(1/magicNumber)*255;
					r = (magicNumber-distance)*(1/magicNumber)*255;
				}
				dataArray[offset + 0] = r;
				dataArray[offset + 1] = g;
				dataArray[offset + 2] = b;
			}
		}
	}*/
	double x1, y1;
	// Here we plot all of the locations
	vector<Entry> locations;
	web->getAllLocations(root, &locations);
	for(int i = 0; i < locations.size(); i++)	{
		x1 = (locations[i].x - (double)x/appWidth)*2; 
		y1 = (locations[i].y - (double)y/appHeight - 0.5)*2;
		circle(dataArray, x1*appWidth, (1-y1)*appHeight, 2, Color8u(255, 215, 0));
	}
	locations.clear();
}

void HW04obissiam_1App::censusMap(uint8_t* picture)	{
	//Here we print out a visualization of the starbucks based on the census data
	// A pink or red pixel indicates that there are a high number of starbucks in that area 
	// compared to population, so new starbucks locations should not be built.
	// A green pixel indicated low starbucks in relation to the population, so new ones can be built.
	// A black pixel indicates a low population and low number of starbucks locations, so new ones are probably not needed.
	// Anything in between is less certain.
	memcpy(dataArray, picture, 3*surfaceSize*surfaceSize);
	ifstream myfile;
	myfile.open("../resources/Census_2010.csv", ifstream::in);
	int region, dummy1, dummy2, numStarbucks, population;
	region = 1; int offset; double x,y;
	if (myfile.is_open())	{
		while (!myfile.eof())	{
			myfile >> region;
			while(myfile.get() != ',' && !myfile.eof());
			myfile >> dummy1;
			while(myfile.get() != ',' && !myfile.eof());
			myfile >> dummy2;
			while(myfile.get() != ',' && !myfile.eof());
			myfile >> numStarbucks;
			while(myfile.get() != ',' && !myfile.eof());
			myfile >> population;
			while(myfile.get() != ',' && !myfile.eof());
			myfile >> x;
			while(myfile.get() != ',' && !myfile.eof());
			myfile >> y;
			offset = 3*((int)(x*appWidth) + (int)((1-y)*appHeight)*surfaceSize);
			if (population==0) population = 1;
			dataArray[offset] += 2*numStarbucks;
			dataArray[offset+1] += population/200;
			dataArray[offset+2] -= (int)(2000*numStarbucks/population);
			
		}
	} else cout << "Unable to open file" << endl;
	myfile.close();
}

void HW04obissiam_1App::mouseDown( MouseEvent event )
{
	Vec2i mousePosition = event.getPos();
	Web* web1 = web;
	double x = (double) mousePosition.x/(appWidth); double y = 1- ((double) mousePosition.y/(appHeight));
	Entry* e = web1->getNearest(x, y);
	if (!leftClicked)	{
		currentStarbucks = e;
		leftClicked = true;
	} else {
		circle((*mySurface_).getData(), currentStarbucks->x*appWidth, (1-currentStarbucks->y)*appHeight, 4, Color8u(255,215,0));
		currentStarbucks = e;
	} 	circle((*mySurface_).getData(), e->x*appWidth, (1-e->y)*appHeight, 4, Color8u(0,255,0));

}

void HW04obissiam_1App::keyDown( KeyEvent event) {
	if (leftClicked)	{
		// For some reason it does not want to zoom with a key event
		if (event.getChar() == 'p')		{
			topRight = true;
		}
		if (event.getChar() == 'o')		{
			currentNode = currentNode->left_->right_;
			//drawMap(currentNode, true, 0, 0, 0.025);
		}
		if (event.getChar() == 'k')		{
			currentNode = currentNode->left_->left_;
			//drawMap(currentNode, true, 0, appHeight/2, 0.025);
		}
		if (event.getChar() == 'l')		{
			currentNode = currentNode->right_->left_;
			//drawMap(currentNode, true, appWidth/2, appHeight/2, 0.025);
		}
	}
}

void HW04obissiam_1App::update()
{	
	if (topRight)	{
		currentNode = currentNode->right_->right_;
		drawMap(dataArray, currentNode, true, appWidth/2, 0, 0.025);
	}
}

void HW04obissiam_1App::draw()
{
	gl::draw(*mySurface_);	
}

CINDER_APP_BASIC( HW04obissiam_1App, RendererGl )
