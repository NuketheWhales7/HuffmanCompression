//Christopher Miller
//CIS-2542

#pragma once

#include <string>

class Node
{
public:
	// Construct a new node
	// The vector doesn't care about this one single bit
	// The set construction absolutely cares
	Node(char c, int v = 1, Node * l = nullptr, Node * r = nullptr);

	// Character data
	char data;
	
	// Compression data
	std::string compression;

	// How often does the character appear in the text?
	int value;
	
	// "Left" pointer
	Node * zero;
	
	// "Right" pointer
	Node * one;
};