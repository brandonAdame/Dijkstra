// CSCI 2530
// Assignment: 6
// Author:     Brandon Gachuz
// File:       pqueue.cpp
// Tab stops:  4

// PUT DESCRIPTION HERE

#include <iostream>
#include <cstdio>
#include "pqueueOriginal.h"

using namespace std;

// PQCell is used as a cell in a linked list. It contains an item, priority
// and a pointer to the next cell in the list. In addition, it has a constructor
// to to initialize each aforementioned field.

struct PQCell
{
    PQItemType item;
    PQPriorityType priority;
    PQCell* next;

    PQCell(PQItemType itm, PQPriorityType pri, PQCell* nxt)
    {
        item = itm;
        priority = pri;
        next = nxt;
    }
};

// Checks if PriorityQueue object 'q' is empty.
// Returns true if 'q' is empty or false otherwise.

bool isEmpty(const PriorityQueue& q)
{
    return q.toLinkedList == NULL;
}

// Inserts a PQCell into the PriorityQueue in ascending order.
// This is a helper function for the function 'insert(q, item, pri)'.

void insertCell(PQCell*& p, const PQItemType& item, PQPriorityType pri)
{
    if (p == NULL)
    {
        p = new PQCell(item, pri, NULL);
    }
    else if (pri < p->priority)
    {
        p = new PQCell(item, pri, p);
    }
    else
    {
        insertCell(p->next, item, pri);
    }
}

// 'insert' inserts item 'item' with priority 'pri' into PriorityQueue object 'q'.

void insert(PriorityQueue& q, PQItemType item, PQPriorityType pri)
{
    insertCell(q.toLinkedList, item, pri);
}

// remove(q, item, pri) removes the item from 'q' that has the smallest priority.
// The function stores the information of the cell removed
// in out-parameters: 'item' and 'pri'.

void remove(PriorityQueue& q, PQItemType& item, PQPriorityType& pri)
{
    if (!isEmpty(q))
    {
        PriorityQueue temp;
        temp.toLinkedList = q.toLinkedList;

        item = q.toLinkedList->item;
        pri = q.toLinkedList->priority;

        q.toLinkedList = temp.toLinkedList->next;
        delete temp.toLinkedList;
    }
}

// printPriorityQueue(q, pi, pp) writes the priority queue of param 'q'
// to standard output.
//
// The purpose of this function is to make sure
// the priority queue is displayed in ascending order.

void printPriorityQueue(const PriorityQueue& q, ItemPrinter pi,
                        PriorityPrinter pp)
{
    PQCell* temp = q.toLinkedList;
    while (temp != NULL)
    {
        pi(temp->item);
        putchar('\n');
        pp(temp->priority);
        cout << "\n\n";
        temp = temp->next;
    }
}
