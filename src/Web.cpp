#include "Web.h"

// Default constructor sets root_ to NULL
Web::Web() {root_ = NULL;}
//Constructor sets root value
Web::Web(Node* root)	{
	root_ = root;
}

// Here we have a Web constructor.  It's job is to create the initial nodes for the structure.
Web::Web(Node* root, char numBits)	{
	root_  = root;
	char test;
	int iterations = pow(2.0, numBits);
	int count = 0;
	Node* temp;
	for (int i = 0; i < iterations; i++)	{
		for (int j = 0; j < iterations; j++)	{
			Node* cur = root;
			for (int k = numBits; k > 0; k--)	{
				//bit = number & (1 << x);
				// Check to see if kth bit of i is greater than 0;
				test = i& (1 << k - 1);
				// Insert new node (insertNode checks if there is already a node on it's own
				temp = cur->insertNode(cur, new Entry(), (test == 0));
				// Move cur down the web
				cur = temp;
				// Clear test bit so it equals 0 again
				test &= ~(1 << k - 1); 

				// Repeat for j
				test = j& (1 << k - 1); 
				temp = cur->insertNode(cur, new Entry(), (test == 0));
				cur = temp;
				test &= ~(1 << k - 1); 
			}
		}
	}
}

// Here we give spatial information about the node.  There are four types of nodes, and 
// they branch differently.
void Web::defineCorner(Node* root, bool leftOrRight)	{
	// Case root is left
	if (leftOrRight == true)	{
		if (root->exists(root, true))	{
			root->left_->nodeInfo_ |= 0 << LOCATIONBIT0;
			root->left_->nodeInfo_ |= 0 << LOCATIONBIT1;
			root->right_->nodeInfo_ |= 0 << LOCATIONBIT0;
			root->right_->nodeInfo_ |= 1 << LOCATIONBIT1;
			defineCorner(root->left_, true);
			defineCorner(root->right_, false);
		} else return;
	// Case root is right
	} else	{
	if (root->exists(root, false))	{
			root->left_->nodeInfo_ |= 1 << LOCATIONBIT0;
			root->left_->nodeInfo_ |= 0 << LOCATIONBIT1;
			root->right_->nodeInfo_ |= 1 << LOCATIONBIT0;
			root->right_->nodeInfo_ |= 1 << LOCATIONBIT1;
			defineCorner(root->left_, true);
			defineCorner(root->right_, false);
		} else return;
	}
}

// Assumes a web of hieght 4 or greater
void Web::insertTethers(Node* root, char size)	{
	// Each grandchild will have pointers to other nodes surounding them.
	if (size > 1)	{
		char newSize = size - 2;
		Node* zero = root->left_->left_;
		Node* one = root->left_->right_;
		Node* two = root->right_->left_;
		Node* three = root->right_->right_;
		// Points to the seven surrounding blocks
		zero->middle_Left_ = zero->getLeft(zero, 0);
		zero->middle_Right_ = zero->getRight(zero, 0);
		zero->top_Right_ = zero->getTL(zero, 0);
		zero->top_Middle_ = zero->getTM(zero, 0);
		zero->top_Right_ = zero->getTR(zero, 0);
		zero->bottom_Right_ = zero->getBL(zero, 0);
		zero->bottom_Middle_ = zero->getBM(zero, 0);
		zero->bottom_Right_ = zero->getBR(zero, 0);

		one->middle_Left_ = one->getLeft(one, 1);
		one->middle_Right_ = one->getRight(one, 1);
		one->top_Right_ = one->getTL(one, 1);
		one->top_Middle_ = one->getTM(one, 1);
		one->top_Right_ = one->getTR(one, 1);
		one->bottom_Right_ = one->getBL(one, 1);
		one->bottom_Middle_ = one->getBM(one, 1);
		one->bottom_Right_ = one->getBR(one, 1);

		two->middle_Left_ = two->getLeft(two, 2);
		two->middle_Right_ = two->getRight(two, 2);
		two->top_Right_ = two->getTL(two, 2);
		two->top_Middle_ = two->getTM(two, 2);
		two->top_Right_ = two->getTR(two, 2);
		two->bottom_Right_ = two->getBL(two, 2);
		two->bottom_Middle_ = two->getBM(two, 2);
		two->bottom_Right_ = two->getBR(two, 2);

		three->middle_Left_ = three->getLeft(three, 3);
		three->middle_Right_ = three->getRight(three, 3);
		three->top_Right_ = three->getTL(three, 3);
		three->top_Middle_ = three->getTM(three, 3);
		three->top_Right_ = three->getTR(three, 3);
		three->bottom_Right_ = three->getBL(three, 3);
		three->bottom_Middle_ = three->getBM(three, 3);
		three->bottom_Right_ = three->getBR(three, 3);

		// If we haven'treached the end of the web, the method is called recursively.
		insertTethers(zero, newSize);
		insertTethers(one, newSize);
		insertTethers(two, newSize);
		insertTethers(three, newSize);
	} else return;
}

// This function is the workhorse of this class. It uses the other functions and constructors to create the 
// web, then adds the array of Entry objects.
void Web::build(Entry* c, int n) {
	// Creat the initail web structure
	Web* web = new Web(root_, NUMBITS);
	// Set values at first two child nodes
	root_->left_->nodeInfo_ |= 0 << LOCATIONBIT0;
	root_->left_->nodeInfo_ |= 0 << LOCATIONBIT1;
	root_->right_->nodeInfo_ |= 0 << LOCATIONBIT0;
	root_->right_->nodeInfo_ |= 1 << LOCATIONBIT1;	
	// Set values at all the other nodes so we know which 
	// surrounding nodes to point to.  
	web->defineCorner(root_->left_, true);
	web->defineCorner(root_->right_, false);
	// Tie nodes together to form web
	web->insertTethers(root_->left_->left_, 2*NUMBITS - 2);
	web->insertTethers(root_->left_->right_, 2*NUMBITS - 2);
	web->insertTethers(root_->right_->left_, 2*NUMBITS - 2);
	web->insertTethers(root_->right_->right_, 2*NUMBITS - 2);
	// Local variables to help read in values from parameters
	unsigned int testX, testY;
	double inX, inY;
	// The current node points to the root;
	Node* cur = root_;
	for (int i = 0; i < n; i++)	{
		inX = c[i].x;
		inY = c[i].y;
		// Shift the double bits over for use when we look for the leaf node.
		testX = (unsigned int) pow(2.0, NUMBITS) * inX;
		testY = (unsigned int) pow(2.0, NUMBITS) * inY;
		// Start from the root.
		cur = root_;
		// Deciding where in the web the Entry should go.
		// It will always go to a node at the edge of our web.
		for (int j = NUMBITS - 1; j >= 0; j--)	{
			// The first bits of the x coordinate choose which way the child node goes
			cur = cur->moveDown(cur, ((testX& (1 << j)) == 0));
			cur->nodeInfo_ |= 1 << ELEMENTADDED;
			// The first bits of y choose which way the grandchild node is
			cur = cur->moveDown(cur, ((testY& (1 << j)) == 0));
			cur->nodeInfo_ |= 1 << ELEMENTADDED;
		}
		// Inserts a node with our entry at the desired location
		cur->leafAdded(cur, &c[i]); 
	}
}

Entry* Web::getNearest(double x, double y) {
	// The current node points to the root;
	Node* cur = root_; Entry* e = new Entry();
	e->x = x; e->y = y;
	// Work our way down to the appropriate node
	unsigned int xInt = (unsigned int) pow(2.0, NUMBITS) * x;
	unsigned int yInt = (unsigned int) pow(2.0, NUMBITS) * y;
	for (int i = NUMBITS - 1; i >= 0; i--)	{
		cur = cur->moveDown(cur, ((xInt& (1 << i)) == 0));
		cur = cur->moveDown(cur, ((yInt& (1 << i)) == 0));
	}
	cur = cur->searchThreads(cur, e);
	return cur->data_;
}