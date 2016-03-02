/*
 *  List.h
 *
 *  Library implementing a generic, dynamic queue (linked list version).
 *
 *  ---
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *  ---
 *
 *  Version 1.0
 *
 *    2010-09-28  Efstathios Chatzikyriakidis  <contact@efxa.org>
 *
 *      - added exit(), blink(): error reporting and handling methods.
 *
 *    2010-09-25  Alexander Brevig  <alexanderbrevig@gmail.com>
 *
 *      - added setPrinter(): indirectly reference a Serial object.
 *
 *    2010-09-20  Efstathios Chatzikyriakidis  <contact@efxa.org>
 *
 *      - initial release of the library.
 *
 *  ---
 *
 *  For the latest version see: http://www.arduino.cc/
 */

// header defining the interface of the source.
#ifndef _LIST_H
#define _LIST_H

// include Arduino basic header.
#include <Arduino.h>

// the definition of the queue class.
template<typename T>
class List {
  public:
    // init the queue (constructor).
    List ();

    // clear the queue (destructor).
    ~List ();

    // push an item to the queue.
    void push (const T i);

    // pop an item from the queue.
    T pop ();

    // get an item from the queue.
    T peek () const;

	void remove(int);
    // check if the queue is empty.
    bool isEmpty () const;

    // get the number of items in the queue.
    int count () const;

	// the structure of each node in the list.
    typedef struct node {
      T item;      // the item in the node.
      node * next; // the next node in the list.
    } node;

    typedef node * link; // synonym for pointer to a node.
    link head;       // the head of the list.
    link tail;       // the tail of the list.

  private:
    int size;        // the size of the queue.
};

// init the queue (constructor).
template<typename T>
List<T>::List () {
  size = 0;       // set the size of queue to zero.
  head = NULL;    // set the head of the list to point nowhere.
  tail = NULL;    // set the tail of the list to point nowhere.
}

// clear the queue (destructor).
template<typename T>
List<T>::~List () {
  // deallocate memory space of each node in the list.
  for (link t = head; t != NULL; head = t) {
    t = head->next; delete head;
  }

  size = 0;       // set the size of queue to zero.
  tail = NULL;    // set the tail of the list to point nowhere.
}

// push an item to the queue.
template<typename T>
void List<T>::push (const T i) {
  // create a temporary pointer to tail.
  link t = tail;

  // create a new node for the tail.
  tail = (link) new node;

  // if there is a memory allocation error.
  if (tail == NULL)
    return;

  // set the next of the new node.
  tail->next = NULL;

  // store the item to the new node.
  tail->item = i;

  // check if the queue is empty.
  if (isEmpty ())
    // make the new node the head of the list.
    head = tail;
  else
    // make the new node the tail of the list.
    t->next = tail;
  
  // increase the items.
  size++;
}

// pop an item from the queue.
template<typename T>
T List<T>::pop () {
  // check if the queue is empty.
  if (isEmpty ())
    return NULL;

  // get the item of the head node.
  T item = head->item;

  // remove only the head node.
  link t = head->next; delete head; head = t;

  // decrease the items.
  size--;

  // return the item.
  return item;
}

// get an item from the queue.
template<typename T>
T List<T>::peek () const {
  // check if the queue is empty.
  if (isEmpty ())
    return NULL;

  // return the item of the head node.
  return head->item;
}

template<typename T>
void List<T>::remove (int index) {
	link t;
	if(count() == 0) return;
	if(index == 0)
	{
		t = head->next;
		delete head;
		head = t;
		size--;
		if(size == 0) tail = NULL;
	}
	else
	{
		index--;
		link cur = head;
		do
		{
			if(index == 0)
			{
				t = cur->next->next;
				if(tail == cur->next) tail = cur;
				delete cur->next;
				cur->next = t;
				size--;
				return;
			}
			index--;
			cur = cur->next;
		}
		while(cur);
	}
}

// check if the queue is empty.
template<typename T>
bool List<T>::isEmpty () const {
  return head == NULL;
}

// get the number of items in the queue.
template<typename T>
int List<T>::count () const {
  return size;
}


#endif // _LIST_H
