#include "HW04obissiam_1App.h"

void HW04obissiam_1App::setup()
{
	mySurface_ = new Surface(surfaceSize,surfaceSize,false);
	Surface US_picture(loadImage(loadResource(RES_US_PIC)));
	uint8_t* pic_data = US_picture.getData();	
	uint8_t* pic_data_pattern = new uint8_t[appWidth*appHeight*3];
	uint8_t* dataArray = (*mySurface_).getData();
	imageToSurface(dataArray, pic_data);

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
			circle(dataArray, x*appWidth, (1-y)*appHeight, 4, Color8u(255,0,0));
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

void HW04obissiam_1App::mouseDown( MouseEvent event )
{
}

void HW04obissiam_1App::update()
{
}

void HW04obissiam_1App::draw()
{
	// clear out the window with black
	//gl::clear( Color( 0, 0, 0 ) );
	gl::draw(*mySurface_);
	
}

CINDER_APP_BASIC( HW04obissiam_1App, RendererGl )
