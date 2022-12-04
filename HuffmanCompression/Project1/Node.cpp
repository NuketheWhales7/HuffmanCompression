//Christopher Miller
//CIS-2542

#include "Node.h"

// Construct a node object
Node::Node(char c, int v, Node * l, Node * r)
	: data(c),
	  compression(""),
	  value(v),
	  zero(l),
	  one(r)
{
	// Nothing here...
}