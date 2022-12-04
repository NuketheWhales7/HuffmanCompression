//Christopher Miller
//CIS-2542

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include "Functors.h"
#include "Node.h"

void PrintCustomSet(const Node * const node)
{
	// End the recursion if we hit a dead end
	// In-Order traversal otherwise
	if (node != nullptr)
	{
		// Go to the left
		PrintCustomSet(node->zero);
		
		// Print, but only if it is a character data node
		if (node->data >= 32)
		{
			std::cout << node->data << " " << node->compression << " " << std::endl;
		}
		
		// Go to the right
		PrintCustomSet(node->one);
	}
}

void GenerateCompressionStrings(Node * const node, const std::string & original)
{
	// End the recursion if we hit a dead end
	// In-Order traversal otherwise
	if (node != nullptr)
	{
		// Go to the left
		std::string modified = original + "0";
		GenerateCompressionStrings(node->zero, modified);

		// We have the compression string ready for this node
		node->compression = original;

		// Go to the right
		modified = original + "1";
		GenerateCompressionStrings(node->one, modified);
	}
}

std::set<Node *, NodeSortingTest> GenerateSTLSetFromStringDataInFile(const std::string & filename)
{
	// Open text file full of random words
	std::ifstream file(filename);
	if (file.fail())
	{
		// Exception if the file is not found
		throw "INPUT FILE DID NOT OPEN CORRECTLY!";
	}
	
	// The STL set will maintain all used characters and there number of occurrences
	std::set<Node *, NodeSortingTest> s;

	while(file.eof() == false)
	{
		// Read every character in the file
		char c;
		file >> std::noskipws >> c;
		// Try to find the Node object in the set, using the character as the search value
		std::set<Node *>::iterator iter = std::find_if(s.begin(), s.end(), NodeEqualityTest(toupper(c)));
		if (iter != s.end())
		{
			// If it's found already in the set, increment the occurrence value...
			++(*iter)->value;
		}
		else
		{
			// Otherwise it's new to the set
			s.insert(new Node(toupper(c)));
		}
	}
	
	// Close the file and return the set of character data
	file.close();
	return s;
}

Node * GenerateCustomSetFromVectorOfNodes(const std::set<Node *, NodeSortingTest> & characterData)
{
	// Create a priority queue that sorts by character repetition value
	std::priority_queue<Node *, std::vector<Node *>, NodeSortingTest> queue;

	// Place every Node in the queue sorted in this way
	for (Node * n : characterData)
	{
		queue.push(n);
	}

	// When you are reduced to one Node you got a binary tree (set) going!
	while (queue.size() > 1)
	{
		// Take the first two Nodes in the queue and parent them to a new node...
		Node * a = queue.top();
		queue.pop();
		Node * b = queue.top();
		queue.pop();
		Node * parent = new Node(0, a->value + b->value, a, b);
		// Push the parent back into the queue with the new repetition value
		queue.push(parent);
	}

	// Generate the compression strings now that the binary tree (set) has been constructed
	std::string emptyReference("");
	GenerateCompressionStrings(queue.top(), emptyReference);
	
	// The binary tree is constructed and each node now has its compression string ready to go
	return queue.top();
}

std::string FindCompressionStringFromCharacter(const Node * const node, char c)
{
	// Obvious case
	if (node == nullptr)
	{
		return "";
	}

	// If found, the compression data is already part of the Node object
	if (node->data == c)
	{
		return node->compression;
	}

	// Otherwise try to find the compression string in the left subtree...
	std::string zero = FindCompressionStringFromCharacter(node->zero, c);
	if (zero != "")
	{
		return zero;
	}

	// Or maybe try the right one too
	return FindCompressionStringFromCharacter(node->one, c);
}

std::string EncodeString(const Node * const node, const std::string & phrase)
{
	// For every character in the phrase, add its compression string 
	// to the end of the return string
	std::string code = "";
	for (char c : phrase)
	{
		code += FindCompressionStringFromCharacter(node, toupper(c));
	}
	return code;
}

std::string DecodeString(const Node * const node, const std::string & filename)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << "File Failed" << std::endl;
		exit(1);
	}

	std::vector<char> input;
	while (file.eof() == false)
	{
		char c;
		file >> c;
		input.push_back(c);
	}
	file.close();

	std::string codephrase;
	const Node* current = node;
	for (char character : input)
	{
		if (character == '0')
		{
			current = current->zero;
		}
		else
		{
			current = current->one;
		}
		if ((current->zero == nullptr) && (current->one == nullptr))
		{
			codephrase += current->data;
			current = node;
		}
	}

	// What is the mystery phrase?
	// TODO:  Find out!
	return codephrase;
}

void DestroyCustomSet(const Node * const node)
{
	// Recursively destroy children, then yourself
	if (node != nullptr)
	{
		DestroyCustomSet(node->zero);
		DestroyCustomSet(node->one);
		delete node;
	}
}

int main()
{
	// Open the sample text file and generate the STL set of data
	std::set<Node *, NodeSortingTest> characterData = GenerateSTLSetFromStringDataInFile("testdata.txt");

	// Take the STL set and generate a custom set from it
	// This set will contain the compression strings for each character
	Node * set = GenerateCustomSetFromVectorOfNodes(characterData);

	// Print the entire binary tree (set) starting from the root node
	PrintCustomSet(set);

	// Compress a string!
	std::string test = "My computer is so slow it Hertz!";
	std::string code = EncodeString(set, test);
	std::cout << std::endl << test << " COMPRESSES TO: " << std::endl;
	std::cout << code << std::endl;

	// TODO:  Decompress the mystery string found in this text file!
	std::cout << std::endl << "THE SECRET TEXT FILE DECOMPRESSES TO: " << std::endl;
	code = DecodeString(set, "secret.txt");
	std::cout << code << std::endl << std::endl;

	// Release dynamic memory
	DestroyCustomSet(set);
	set = nullptr;

	// Finally done!
	system("PAUSE");
	return 0;
}