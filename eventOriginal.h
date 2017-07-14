// CSCI 2530
// Assignment: 6
// Author:     Brandon Gachuz
// File:       event.h
// Tab stops:  4

#ifndef EVENT_H
#define EVENT_H

/*

 Event structure holds a sender, receiver and a time. Event also
 has a constructor to initialize each aforementioned field.

 */

struct Event
{
    double time;
    int sender;
    int receiver;

    Event(int send, int rec, double tm)
    {
        sender = send;
        receiver = rec;
        time = tm;
    }
};

#endif
