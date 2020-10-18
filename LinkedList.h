/*		Doubly Linked List
	* Author: Brandon Grunes
*/
#pragma once
#include <vector>
#include <iostream>
#include "leaker.h"
using std::cout;
using std::endl;
using std::vector;
using std::out_of_range;

template <typename T>
class LinkedList
{

public:
	struct Node
	{
		T data;
		Node* next = nullptr;
		Node* prev = nullptr;
	};

private:
	Node* head;
	Node* tail;
	unsigned int numNodes;

public:

	/*===== Construction/Destruction =====*/
	LinkedList();
	LinkedList(const LinkedList<T>& list);
	~LinkedList();

	/*===== Operators =====*/
	const T& operator[](unsigned int index) const;
	T& operator[](unsigned int index);
	bool operator==(const LinkedList<T>& rhs) const;
	LinkedList<T>& operator=(const LinkedList<T>& rhs);

	/*===== Insertion =====*/
	void AddHead(const T& data);
	void AddTail(const T& data);
	void AddNodesHead(const T* data, unsigned int count);
	void AddNodesTail(const T* data, unsigned int count);
	void InsertAfter(Node* node, const T& data);
	void InsertBefore(Node* node, const T& data);
	void InsertAt(const T& data, unsigned int index);

	/*===== Removal =====*/
	bool RemoveHead();
	bool RemoveTail();
	unsigned int Remove(const T& data);
	bool RemoveAt(unsigned int index);
	void Clear();

	/*===== Behaviors =====*/
	void PrintForward() const;
	void PrintReverse() const;
	void PrintForwardRecursive(const Node* node) const;
	void PrintReverseRecursive(const Node* node) const;

	/*===== Accessors =====*/
	unsigned int NodeCount() const;
	void FindAll(vector<Node*>& outData, const T& value) const;
	const Node* Find(const T& data) const;
	Node* Find(const T& data);
	const Node* GetNode(unsigned int index) const;
	Node* GetNode(unsigned int index);
	Node* Head();
	const Node* Head() const;
	Node* Tail();
	const Node* Tail() const;

};

/*===== Construction/Destruction =====*/
template <typename T>
LinkedList<T>::LinkedList()
{
	head = nullptr;
	tail = nullptr;
	numNodes = 0;
}

// Copy Constructor
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list)
{

	Node* listNode = list.head;

	Node* headNode = new Node;
	headNode->data = listNode->data;

	head = headNode;

	this->head->next = nullptr;

	Node* current = this->head;

	listNode = listNode->next;

	while (listNode != nullptr)
	{
		current->next = new Node;
		current = current->next;

		current->data = listNode->data;

		current->next = nullptr;

		listNode = listNode->next;
	}

	tail = current;
}

// Destructor
template <typename T>
LinkedList<T>::~LinkedList()
{
	Clear();
}

/*===== Operators =====*/
// Index Operator
template <typename T>
const T& LinkedList<T>::operator[] (unsigned int index) const
{
	if (index >= numNodes)
		throw out_of_range("Index out of range");

	Node* current = head;
	for (unsigned int i = 1; i <= index; i++)
		current = current->next;

	return current->data;
}

// Index operator
template <typename T>
T& LinkedList<T>::operator[] (unsigned int index)
{
	if (index > numNodes)
		throw out_of_range("Index out of range");

	Node* current = head;
	for (unsigned int i = 1; i <= index; i++)
		current = current->next;

	return current->data;
}

// Comparison Operator
template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& rhs) const
{
	if (head->data != rhs.head->data)
		return false;
	else if (tail->data != rhs.tail->data)
		return false;

	Node* current = head;
	Node* rhs_current = rhs.head;
	for (unsigned int i = 0; i < rhs.numNodes; i++)
	{
		if (current->data != rhs_current->data)
			return false;
		else
		{
			current = current->next;
			rhs_current = rhs_current->next;
		}
	}

	return true;
}

// Copy assignment operator
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& rhs)
{
	Clear();

	Node* listNode = rhs.head;

	Node* headNode = new Node;
	headNode->data = listNode->data;

	head = headNode;

	this->head->next = nullptr;

	Node* current = this->head;

	listNode = listNode->next;

	while (listNode != nullptr)
	{
		current->next = new Node;
		current = current->next;

		current->data = listNode->data;

		current->next = nullptr;

		listNode = listNode->next;
	}

	tail = current;

	return *this;
}

/*===== Insertion =====*/
// Add a head node
template <typename T>
void LinkedList<T>::AddHead(const T& data)
{
	Node* headNode = new Node;
	headNode->data = data;

	if (head == nullptr)
	{
		head = headNode;
		tail = head;
		head->next = nullptr;
		head->prev = nullptr;
	}
	else
	{
		headNode->next = head;
		head->prev = headNode;
		head = headNode;
	}

	numNodes++;
}
// Add a tail node
template <typename T>
void LinkedList<T>::AddTail(const T& data)
{
	Node* tailNode = new Node;
	tailNode->data = data;

	if (tail == nullptr)
	{
		tail = tailNode;
		head = tail;
		tail->next = nullptr;
		tail->prev = nullptr;
	}
	else
	{
		tailNode->next = nullptr;
		tailNode->prev = tail;
		tail->next = tailNode;
		tail = tailNode;

		if (head == nullptr)
		{
			Node* current = tail;
			for (unsigned int i = 1; i <= numNodes; i++)
				current = current->prev;

			head = current;
		}
	}
	numNodes++;
}

// Add a list of nodes before the original list
template <typename T>
void LinkedList<T>::AddNodesHead(const T* data, unsigned int count)
{
	AddHead(data[0]);

	Node* current = head;

	for (unsigned int i = 1; i < count; i++)
	{
		InsertAfter(current, data[i]);
		current = current->next;
	}
}

// Add a list of nodes onto the original list after the tail
template <typename T>
void LinkedList<T>::AddNodesTail(const T* data, unsigned int count)
{
	Node* current = tail;

	for (unsigned int i = 0; i < count; i++)
	{
		InsertAfter(current, data[i]);
		current = current->next;
	}
}

// Insert a node after the specified node with passed in parameter data
template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& data)
{
	Node* newNode = new Node;
	newNode->data = data;
	newNode->next = node->next;
	newNode->prev = node;
	node->next = newNode;

	if (newNode->next == nullptr)
		tail = newNode;
	else
	{
		Node* current = newNode->next;
		current->prev = newNode;
	}

	numNodes++;
}

// Insert a node before the specified node with passed in parameter data
template <typename T>
void LinkedList<T>::InsertBefore(Node* node, const T& data)
{
	Node* newNode = new Node;
	newNode->data = data;

	if (node == head)
	{
		node->prev = newNode;
		newNode->next = node;
		newNode->prev = nullptr;
		head = newNode;
	}
	else
	{
		Node* current = node->prev;
		current->next = newNode;

		newNode->next = node;
		newNode->prev = node->prev;
		node->prev = newNode;
	}

	numNodes++;
}

// Insert a node at a specified index
template <typename T>
void LinkedList<T>::InsertAt(const T& data, unsigned int index)
{
	// Checks if the index is greater than the number of nodes
	if (index > numNodes)
		throw out_of_range("Index is out of bounds");
	if (index == 0)
		AddHead(data);
	else if (index == numNodes)
		AddTail(data);
	else
	{
		Node* current = head;
		for (unsigned int i = 1; i <= index; i++)
			current = current->next;

		InsertBefore(current, data);
	}
}

// Remove the head node
template <typename T>
bool LinkedList<T>::RemoveHead()
{
	// Check if list of nodes is empty
	if (head == nullptr)
		return false;
	else if (head->next == nullptr)
	{
		Node* temp = head;
		delete temp;
		head = nullptr;
		tail = nullptr;
		numNodes--;
		return true;
	}
	else
	{
		Node* temp = head;
		head->next->prev = nullptr;
		head = head->next;
		delete temp;

		numNodes--;

		return true;
	}
}

// Remove the tail node
template <typename T>
bool LinkedList<T>::RemoveTail()
{
	if (tail == nullptr)
		return false;
	else if (tail->prev == nullptr)
	{
		Node* temp = tail;
		delete temp;

		tail = nullptr;
		head = nullptr;
		numNodes--;
		return true;
	}
	else
	{
		Node* temp = tail;
		tail->prev->next = nullptr;
		tail = tail->prev;
		delete temp;
		numNodes--;
		return true;
	}
}

// Remove nodes that have the same data value as the passed in parameter
template <typename T>
unsigned int LinkedList<T>::Remove(const T& data)
{
	unsigned int count = 0;
	Node* current = head;
	Node* next = current->next;

	while (next != nullptr)
	{
		if (current->data == data)
		{
			Node* prev = current->prev;
			prev->next = next;

			next->prev = current->prev;


			delete current;
			current = next;
			numNodes--;
			count++;
		}
		else
		{
			current = next;
			next = current->next;
		}
	}
	return count;
}

template <typename T>
// Remove a node at the specified index, decrease the number of nodes
bool LinkedList<T>::RemoveAt(unsigned int index)
{
	Node* current = head;

	// If the index is greater than the amount of nodes, return false
	if (index > numNodes)
		return false;

	// Traverse the list until the node found at index
	for (unsigned int i = 0; i < index; i++)
		current = current->next;

	// Remove the head if the node is the head
	if (current == head)
	{
		RemoveHead();
		return true;
	}
	// Remove the head if the node is the tail
	else if (current == tail)
	{
		RemoveTail();
		return true;
	}
	// Remove the node 
	else
	{
		Node* next = current->next;
		Node* prev = current->prev;

		prev->next = next;
		next->prev = prev;

		numNodes--;

		delete current;

		return true;
	}


	return false;
}

template <typename T>
// Erase the list of nodes and set the number of nodes to 0
void LinkedList<T>::Clear()
{
	Node* current = head;

	while (current != nullptr)
	{
		current = current->next;
		delete head;
		head = current;
	}

	head = nullptr;
	tail = nullptr;

	numNodes = 0;
}

/*===== Behaviors =====*/
// Prints a list of the nodes' values
template <typename T>
void LinkedList<T>::PrintForward() const
{
	Node* current = head;

	while (current != nullptr)
	{
		cout << current->data << endl;
		current = current->next;
	}
}

// Prints a list of the nodes' values in reverse
template <typename T>
void LinkedList<T>::PrintReverse() const
{
	Node* current = tail;

	while (current != nullptr)
	{
		cout << current->data << endl;
		current = current->prev;
	}
}

// Prints a list of values starting from the node given, recursive
template <typename T>
void LinkedList<T>::PrintForwardRecursive(const Node* node) const
{
	if (node == nullptr)
		return;
	else
		cout << node->data << endl;

	PrintForwardRecursive(node->next);

}

// Prints a list of values starting from the node given in reverse, recursive 
template <typename T>
void LinkedList<T>::PrintReverseRecursive(const Node* node) const
{
	if (node == nullptr)
		return;
	else
		cout << node->data << endl;

	PrintReverseRecursive(node->prev);

}

/*===== Accessors =====*/
// Return the number of nodes
template <typename T>
unsigned int LinkedList<T>::NodeCount() const
{
	return numNodes;
}

// Find all occurrences of nodes that hold the value given in the parameter, and place all the nodes into a vector
template <typename T>
void LinkedList<T>::FindAll(vector<Node*>& outData, const T& value) const
{
	Node* current = head;

	for (unsigned int i = 0; i < numNodes; i++)
	{
		if (current->data == value)
		{
			outData.push_back(current);
			current = current->next;
		}
		else
			current = current->next;
	}
}

// Find a node that contains the data and return an unchangeable version of that node
template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) const
{
	Node* nodeWithData = new Node;
	nodeWithData = head;

	// Traverse the list until the node containing the same data as the parameter is found
	for (unsigned int i = 0; i < numNodes; i++)
	{
		if (nodeWithData->data == data)
			return nodeWithData;
		else
			nodeWithData = nodeWithData->next;
	}

	return nodeWithData;
}

// Find a node that contains the data and return the node
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data)
{
	Node* nodeWithData = head;

	// Traverse the list until the node containing the same data as the parameter is found
	for (unsigned int i = 0; i < numNodes; i++)
	{
		if (nodeWithData->data == data)
			return nodeWithData;
		else
			nodeWithData = nodeWithData->next;
	}

	return nodeWithData;
}

template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) const
{
	// Checks if the index is out of bounds, throws exception
	if (index >= numNodes)
		throw out_of_range("Index out of range");

	Node* foundNode = new Node;
	Node* current = head;

	// Return the head node if the index is 0
	if (index == 0)
		return head;
	else
	{
		// Traveerses the list until index number of times
		for (unsigned int i = 0; i <= index; i++)
			current = current->next;

		// Node at current is the node to return
		foundNode = current;
		return foundNode;
	}
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index)
{
	// Checks if the index is out of bounds, throws exception
	if (index >= numNodes)
		throw out_of_range("Index out of range");

	Node* foundNode = new Node;
	Node* current = head;

	// Return the head node if the index is 0
	if (index == 0)
		return head;
	else
	{
		// Traverses the list until index number of times
		for (unsigned int i = 0; i <= index; i++)
			current = current->next;
		
		// Node at current is the node to return
		foundNode = current;
		return foundNode;
	}
}

// Returns head node
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head()
{
	return head;
}

// Returns an unchangeable version of head
template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Head() const
{
	return head;
}

// Returns tail node
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail()
{
	return tail;
}

// Returns an unchangeable version of tail
template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Tail() const
{
	return tail;
}
