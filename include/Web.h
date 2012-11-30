/** Author: Eddie Obissi
This class creates a web-like data structure, whose initial nodes provide spatial organization, 
and whose final nodes have information that a user might like to use.  

The first nodes are set by a constant number of bits, which is predefined and must be at least 2.
This creates a structure of node which make it easy to place information based on spatial location.
These nodes take the form of a binary tree, having left, right and parent nodes that they point too.
However, this is different from a tree because the nodes can also point to other branches.  
Imagine that each node represents a rectangle on a grid: the node points to those rectangles surrounding it.
As the search increases in size, more and more blocks will be needed, and this is why the extra pointers are necessary.

At the end of the structure there are leaves, which are an ordered string of data points.  When searching, these are 
the values that will be examined.

All methods are original, and may be used by anyone.
*/
#include "Node.h"
#include "Starbucks.h"
#include <vector>

#define NUMBITS 4

class Web : public Starbucks
{
	public:
		// Default Constructor
		Web();
		// A constructor method that sets the root of the Web
		Web(Node* root);
		// A construtor method that creates nodes to form the Web, ending with leaves
		Web(Node* root, char numBits);
		// Goes through a Web, starting at the root, and gives it location values that are important to other methods.
		virtual void defineCorner(Node* root, bool leftOrRight);
		// Links values together so that, searching can be accomplished more quickly.
		virtual void insertTethers(Node* root, char size);
		// Builds a Web that ends with Entry values
		virtual void build(Entry* c, int n);
		// Searches for the closest entry to the value we use
		virtual Entry* getNearest(double x, double y);
		// Returns all of the locations after a given root
		virtual void getAllLocations(Node* root, vector<Entry>* locations);


	private:
		Node* root_;
};