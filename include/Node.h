/**
	A class that creates, manipulates, and searches the nodes that are used in the Web class.
	The more complex functions exist here, as they require recursion to function properly.
*/
#include "Starbucks.h"
#include <cmath>

using namespace std;
// Bit values that assist in identifying different properties of the nodes
#define HASLEFTCHILD 0
#define HASRIGHTCHILD 1
#define LOCATIONBIT1 2
#define LOCATIONBIT0 3
#define BEENCHECKED 4
#define ELEMENTADDED 5
// A default value used when searching to ensure the proper Node is returned.
// This value must be larger than the highest value in our input array.
#define FARVALUE 2

class Node	{
public:
	Node* parent_;
	Node* left_;
	Node* right_;
	// These nodes represent the surrounding blocks of points
	Node* top_Left_;
	Node* top_Middle_;
	Node* top_Right_;
	Node* middle_Right_;
	Node* bottom_Right_;
	Node* bottom_Middle_;
	Node* bottom_Left_;
	Node* middle_Left_;
	// The data that we will need when checking these nodes
	Entry* data_;
	char nodeInfo_;

	
	// Create an empty node
	Node();

	// Destructor
	~Node();
	
	// Create a node with data in it
	Node(Entry* in);
	
	// Insert a node with data in the correct spot
	Node* insertNode(Node* parent, Entry* in, bool leftOrRight);

	// Sees if the node has children
	bool exists(Node* node, bool leftOrRight);

	// Points a node towards the appropriate node
	Node* moveDown(Node* node, bool leftOrRight);

	// Adds leaf values. This is only used for entry data, not for the Web structure
	// These nodes will go linearly, but will be organized from closest to the origin to the furthest
	void leafAdded(Node* node, Entry* in);

	// Searches for the closest value to our Entry in a leaf node
	Node* searchLeaf(Node* ancestor, Node* descendant, Entry* in);

	// Navigates the user through the web when searching
	Node* traverseWeb(Node* node, Entry* in);

	// Searches in the surrounding threads which represent blocks of space 
	Node* searchThreads(Node* node, Entry* in);

	// A convenience method used when searching for the closest location
	double distance(Entry* in, Node* node);

	// These methods are used to tell where our node should point. They correspond to points on the web 
	// that would represent areas which are close to the Entries in the node.
	Node* getLeft(Node* node, short locationType)	{
		switch (locationType)	{
		case 0 : return node->parent_->parent_->parent_->left_->left_->right_;
		case 1 : return node->parent_->left_;
		case 2 : return node->parent_->parent_->parent_->parent_->right_->left_->left_->left_;
		case 3 : return node->parent_->left_;
		}
		return node;
	}

	Node* getTL(Node* node, short locationType)	{
		switch (locationType)	{
		case 0 : return node->parent_->parent_->parent_->left_->right_->right_;
		case 1 : return node->parent_->parent_->right_->left_;
		case 2 : return node->parent_->parent_->parent_->parent_->right_->left_->left_->right_;
		case 3 : return node->parent_->parent_->parent_->parent_->right_->left_->left_->left_;
		}
		return node;
	}

		Node* getTM(Node* node, short locationType)	{
		switch (locationType)	{
		case 0 : return node->parent_->parent_->right_->left_;
		case 1 : return node->parent_->parent_->right_->right_;
		case 2 : return node->parent_->parent_->parent_->parent_->right_->right_->left_->left_;
		case 3 : return node->parent_->parent_->parent_->parent_->right_->left_->left_->right_;
		}
		return node;
	}

	Node* getTR(Node* node, short locationType)	{
		switch (locationType)	{
		case 0 : return node->parent_->parent_->right_->right_;
		case 1 : return node->parent_->parent_->parent_->right_->right_->left_;
		case 2 : return node->parent_->parent_->parent_->parent_->right_->right_->left_->right_;
		case 3 : return node->parent_->parent_->parent_->parent_->right_->right_->left_->left_;
		}
		return node;
	}

	Node* getRight(Node* node, short locationType)	{
		switch (locationType)	{
		case 0 : return node->parent_->right_;
		case 1 : return node->parent_->parent_->parent_->right_->left_->left_;
		case 2 : return node->parent_->right_;
		case 3 : return node->parent_->parent_->parent_->right_->right_->left_;
		}
		return node;
	}

	Node* getBR(Node* node, short locationType)	{
		switch (locationType)	{
		case 0 : return node->parent_->parent_->parent_->parent_->left_->right_->right_->right_;
		case 1 : return node->parent_->parent_->parent_->parent_->left_->right_->right_->left_;
		case 2 : return node->parent_->parent_->left_->right_;
		case 3 : return node->parent_->parent_->parent_->right_->left_->left_;
		}
		return node;
	}

	Node* getBM(Node* node, short locationType)	{
		switch (locationType)	{
		case 0 : return node->parent_->parent_->parent_->parent_->left_->right_->right_->left_;
		case 1 : return node->parent_->parent_->parent_->parent_->left_->left_->right_->right_;
		case 2 : return node->parent_->parent_->left_->left_;
		case 3 : return node->parent_->parent_->left_->right_;
		}
		return node;
	}

	Node* getBL(Node* node, short locationType)	{
		switch (locationType)	{
		case 0 : return node->parent_->parent_->parent_->parent_->left_->left_->right_->right_;
		case 1 : return node->parent_->parent_->parent_->parent_->left_->left_->right_->left_;
		case 2 : return node->parent_->parent_->parent_->left_->left_->right_;
		case 3 : return node->parent_->parent_->left_->left_;
		}
		return node;
	}
};