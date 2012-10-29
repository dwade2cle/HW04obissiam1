// HW04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Web.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	//vector <Entry*> myEntries;
	Entry myEntries[7655];
	string line; double x, y;
	int count = 0;
	ifstream myfile("Starbucks_2006.csv");
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
	Entry listValues[1000];
	for(int i = 0; i < 1000; i++)	{
		listValues[i].identifier = "Zero";
		listValues[i].x = .001*i;
		listValues[i].y = .001*i;
	}
	Node* root = new Node();
	Web* web = new Web(root);
	web->build(myEntries, count);
	Entry* e = web->getNearest(0.95, 0.5);
	cout << e->identifier << ' ' << e->x << ' ' << e->y << endl; 

	return 0;
}

