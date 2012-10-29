#include "Entry.h"
	
// Empty entry
Entry::Entry()	{
	x_ = 0;
	y_ = 0;
	locationName_ = "";
}
		
Entry::Entry(string name, int x, int y)	{
	x_ = x;
	y_ = y;
	locationName_ = name;
}
		
int Entry::getX()	{
	return x_;
}
		
int Entry::getY()	{
	return y_;
}
		
string Entry::getName()	{
	return locationName_;
}