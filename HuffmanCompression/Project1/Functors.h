//Christopher Miller
//CIS-2542

#pragma once

#include "Node.h"

// Needed to sort the priority queue by repetition value
class NodeSortingTest
{
public:
	bool operator () (const Node * lhs, const Node * rhs) const;
};

bool NodeSortingTest::operator () (const Node * lhs, const Node * rhs) const
{
	return lhs->value > rhs->value;
}


// Needed for find_if function
// Really "complicated" way to search a vector of Nodes
// for a specific character
class NodeEqualityTest
{
public:
	NodeEqualityTest(char c);
	bool operator () (const Node * rhs) const;

private:
	char data;
};

NodeEqualityTest::NodeEqualityTest(char c)
	: data(c)
{
	// Nothing here...
}

bool NodeEqualityTest::operator () (const Node * rhs) const
{
	return data == rhs->data;
}