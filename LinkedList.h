#pragma once

#include <stdexcept>
#include <sstream>

using namespace std;

template<class T>
class LinkedList {
private:

	/* node
		a node in the list that stores a data point and pointers to the next and previous nodes
		@member value: the data stored in the node
		@member next: a pointer to the next node in the list
		@member prev: a pointer to the previous node in the list
	 */
    class node {
	public:
		/* constructor
			@param value: the value of the node
			@param next: the next node of the list (if not null, its previous node will be set to this)
			@param prev: the previous node of the list (if not null, its next node will be set to this)
		 */
        node(T value, node* next, node* prev)
            : value(value), next(next), prev(prev) {
                if (next != NULL) next->prev = this;
                if (prev != NULL) prev->next = this;
            }
			
		/* destructor
			recursively deletes all nodes linked to this node
			to safely delete a single node, you must set its next node to NULL
		 */	
        ~node() { if (next != NULL) delete next; }
        
		/* setNext
			@param n: the node to be set as next (if not null, its previous node will be set to this)
		 */
        void setNext(node* n) {
            next = n;
            if (n != NULL) n->prev = this;
        }
		
		/* setPrev
			@param n: the node to be set as previous (if not null, its next node will be set to this)
	 	 */
        void setPrev(node* n) {
            prev = n;
            if (n != NULL) n->next = this;
        }
        
        T value;
        node* next;
        node* prev;
    };
    
    node* head;
    node* tail;
    
    int size_;
    static const int NOT_FOUND = -1;
	
public:
    LinkedList<T>() : head(NULL), tail(NULL), size_(0) {}
    ~LinkedList<T>() { delete head; }
    
	/* indexInRange
		checks whether an index is in a given range
		@param index: the index to check
		@param low: the lower bound of the range
		@param high: the upper bound of the range
		@return: true if in range, false otherwise
	 */
    bool indexInRange(int index, int low, int high) {
        return low <= index && index <= high;
    }
    
	/* find
		finds a node by its index
		@param index: the index of the node to find
		@return: a pointer to the node at the given index
	 */
	node* find(int index) {
        node* current = NULL;
		if (indexInRange(index, 0, size_/2)) { // closer to beginning
			current = head;
			for (int i = 0; i < index; i++) 
				if (current != NULL)
					current = current->next;
		}
		else if (indexInRange(index, (size_/2 + 1), size_)) { // closer to end
			current = tail;
			for (int i = size_ - 1; i > index; i--) 
				if (current != NULL)
					current = current->prev;
		}
		if (current == NULL) throw out_of_range(""); // out of range
		return current;
	}
	
	/* find
		finds a node by value
		@param value: the value to search for
		@return: the index of the first element in the list that matches the given value
	 */
    int find(T value) {
        node* current = head;
        bool found = false;
        int index = 0;
        
        while (current != NULL && !found) {
            if (value == current->value) found = true;
            else { current = current->next; index++; }
        }
        if (!found) index = NOT_FOUND;
        
        return index;
    }
	
	/* operator[]
		returns the value of the node at the given index
		@param index: the index to look at
		@return: the value of the element at that index
	 */
	T operator[] (int index) {
		return find(index)->value;
	}
	
	/* insertHead
		inserts a node at the beginning of the list
		@param value: the value to insert
	 */
	void insertHead(T value) {
	    node* current = head;
	    head = new node(value, current, NULL);
	    if (current == NULL) tail = head;
		size_++;
	}

	/* insertTail
		inserts a node at the end of the list
		@param value: the value to insert
	 */
	void insertTail(T value) {
	    node* current = tail;
	    tail = new node(value, NULL, current);
	    if (current == NULL) head = tail;
		size_++;
	}
	
	/* insert
		inserts a node into the list at the given index
		@param value: the value to insert
		@param index: the insertion index
	 */
    void insert(T value, int index) {
        if (index == 0) insertHead(value); // head
        else if (index == size_) insertTail(value); // tail
        else { // middle
            node* current = find(index);
            new node(value, current, current->prev);
			size_++;
        }
    }
    
	/* removeHead
		removes a node from the beginning of the list
	 */
	T removeHead() {
		T value;

		if (head == NULL) throw out_of_range("");
		else value = head->value;

	    if (head == tail) tail = NULL;
	    node* current = head;
	    head = head->next;

	    current->setNext(NULL);
	    if (head != NULL) head->setPrev(NULL);
	    delete current;
		size_--;
		return value;
	}

	/* removeTail
		removes a node from the end of the list
	 */
	T removeTail() {
		T value;

		if (tail == NULL) throw out_of_range("");
		else value = tail->value;

	    if (tail == head) head = NULL;
	    node* current = tail;
	    tail = tail->prev;

	    current->setPrev(NULL);
	    if (tail != NULL) tail->setNext(NULL);
	    delete current;
		size_--;
		return value;
	}
	
	/* remove
		removes the node at the given index
		@param index: the index of the node to remove
		@return: the value of the removed node
	 */
    T remove(int index) {
        T value;
        if (index == 0) { // head
            value = removeHead();
        }
        else if (index == size_ - 1) { // tail
            value = removeTail();
        }
		else if (index == size_) throw out_of_range(""); // at the end
        else { // middle
            node* current = find(index);
            
            current->prev->setNext(current->next);
            current->setNext(NULL);
			
            value = current->value;
            delete current;
			size_--;
        }
        return value;
    }
    
	/* clear
		clears the list, deleting all nodes
	 */
    void clear() { 
		node* current = head;
		head = NULL;
		tail = NULL;
		delete current; 
		size_ = 0;
	}
    
    int size() { return size_; }
    
	/* toString
		@return: a string representation of the list, in the form "Node n: <data>\n" for each node
	 */
    string toString() {
        stringstream ss;
        
        node* current = head;
        int index = 0;
        while(current != NULL) {
            ss << "node " << index << ": " << current->value << endl;
            current = current->next;
            index++;
        }
        
        return ss.str();
    }
};