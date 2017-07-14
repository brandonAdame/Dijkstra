// CSCI 2530
// Assignment: 6
// Author:     Brandon Gachuz
// File:       dijkstra.cpp
// Tab stops:  4

/*

 Dijkstra is a module designed to find the shortest path between
 two given vertices. The module shows through which vertices it went through
 and the time it took for it to arrive. In addition, it implements tracing
 to see what the program is doing in real-time.

 Functions this program handles include:

 Graph readGraph();
 void printAdjList(const Graph g);
 void printGraph(const Graph g);
 void list(Edge* t);
 void insertEdge(int first, int second, int weight, Graph& g);
 void singleSignal(int u, int v, double time, PriorityQueue& q);
 void multipleSignals(Graph& g, Vertex& v, PriorityQueue& q);
 void processEvent(Graph& g, PriorityQueue& q, Event* e);
 void dijkstraAlgorithm(PriorityQueue& q, Graph& g, int start, int end);
 double shortestDistance(Graph& g, int arg);
 void showPath(Graph&g, int strt, int end);
 Event* getEvent(PriorityQueue& q);
 void setTracing(int argc, char** argv);

 */

#include <cstdio>
#include <iostream>
#include "pqueueOriginal.h"
#include <trace.h>

using namespace std;

// Trace variable used for tracing throughout module.

bool trace = false;

/*
 Type Edge is used as a cell in an adjacency list. The Edge structure stores:
 - Two vertex numbers 'u' and 'v'.
 - A weight.
 - A pointer to the next Edge in the list.

 Type Edge also contains a structure which initializes
 each of the aforementioned fields.

 */

struct Edge
{
    int u; //start
    int v; //end
    double weight;
    Edge* next;

    Edge(int one, int two, double w, Edge* nxt)
    {
        u = one;
        v = two;
        weight = w;
        next = nxt;
    }
};

/*

 A vertex in the graph contains the following:
 - A pointer to the adjacency list of edges
 - A real number indicating v's shortest distance from the start vertex.
 - A boolean that tells whether the signal has reached 'v'
 - An int 'prev' that holds the previous number

 */

struct Vertex
{
    Edge* adjList;
    double distance;
    bool signaled;
    int prev;

    Vertex()
    {
        signaled = false;
        adjList = NULL;
        distance = -1.0;
    }
};

/*

 The Graph structure holds three fields:
 - Number of vertices
 - Number of edges
 - An array of vertices

 The structure also contains a constructor 'Graph(int numV)' which initializes
 the variable for the number of vertices, sets the number of edges to '0'
 and declares the size of the array to be the number of vertices.

 */

struct Graph
{
    int numVerts;
    int numOfEdges;
    Vertex* vertices;

    Graph(int numV)
    {
        numVerts = numV;
        numOfEdges = 0;
        vertices = new Vertex[numVerts+1];
    }
};

// Prototype Functions

Graph readGraph();
void printAdjList(const Graph g);
void printGraph(const Graph g);
void list(Edge* t);
void insertEdge(int first, int second, int weight, Graph& g);
void singleSignal(int u, int v, double time, PriorityQueue& q);
void multipleSignals(Graph& g, Vertex& v, PriorityQueue& q);
void processEvent(Graph& g, PriorityQueue& q, Event* e);
void dijkstraAlgorithm(PriorityQueue& q, Graph& g, int start, int end);
double shortestDistance(Graph& g, int arg);
void showPath(Graph&g, int strt, int end);
Event* getEvent(PriorityQueue& q);
void setTracing(int argc, char** argv);

int main(int argc, char** argv)
{
    setTracing(argc, argv);
    Graph g = readGraph();
    PriorityQueue q;

    int start, finish;
    cin >> start >> finish;

    dijkstraAlgorithm(q, g, start, finish);

    printGraph(g);
    putchar('\n');

    printf("The shortest path from %i to %i "
           "has length %lf and is\n", start, finish,
           shortestDistance(g, finish));
    showPath(g, finish, start);

    putchar('\n');

    return 0;
}

// setTracing turns on tracing for the dijkstra module

void setTracing(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-t") == 0) {
            trace = true;
        }
    }
}

/*
 The first number entered indicates the number
 of vertices the graph will have.

 Then the function requires three values:
 two integer values (separated by a space)
 and a double value separated by a space from the previous integer.

 To end the input, enter '0' to end the input.

 //----------------

 Example input:

 5
 1 2  9.0
 1 3 12.0
 2 4 18.0
 2 3  6.0
 2 5 20.0
 3 5 15.0
 0

 Example output:

 There are 5 vertices and 6 edges.
 The edges are as follows.

 (1, 3) weight 12.000000
 (1, 2) weight 9.000000
 (2, 5) weight 20.000000
 (2, 3) weight 6.000000
 (2, 4) weight 18.000000
 (3, 5) weight 15.000000

 //----------------
 */

Graph readGraph()
{
    int verts;
    cin >> verts;
    Graph g = Graph(verts);
    int startVert, endVert;
    double weight;
    cin >> startVert;
    while (startVert != 0)
    {
        cin >> endVert >> weight;
        insertEdge(startVert, endVert, weight, g);
        cin >> startVert;
    }
    return g;
}

// insertEdge inserts an edge cell into the adjacency list of graph 'g'.

void insertEdge(int first, int second, int weight, Graph& g)
{
    g.vertices[first].adjList =
    new Edge(first, second, weight, g.vertices[first].adjList);

    g.vertices[second].adjList =
    new Edge(second, first, weight, g.vertices[second].adjList);

    g.numOfEdges += 1;
}

// printGraph prints description the graph of 'g' to the standard output.

void printGraph(const Graph g)
{
    cout << "\n\n";
    cout << "There are " << g.numVerts << " vertices and "
    << g.numOfEdges << " edges." << endl;

    cout << "The edges are as follows.\n" << endl;

    printAdjList(g);
}

// printAdjList(g) prints the adjacency list in 'g' to the standard output.

void printAdjList(const Graph g)
{
    for (int i = 1; i < g.numVerts+1; ++i) {
        Edge* temp = g.vertices[i].adjList;
        list(temp);
    }
}

// list(t) access the adjacency list in 't' and
// prints it to the standard output.

void list(Edge* t)
{
    while (t) {
        if (t->u < t->v)
        {
            printf("(%i, %i) weight %lf\n", t->u, t->v, t->weight);
        }
        t = t->next;
    }
}

/*

 singleSignal(int u, int v, double time, PriorityQueue q)
 sends a signal from u to v, arriving at time t, by inserting an event into q.

 */

void singleSignal(int u, int v, double time, PriorityQueue& q)
{
    Event* e = new Event(u, v, time);

    if (trace) {
        printf("Sending signal from %i to %i time %lf", u , v, time);
    }

    insert(q, e, time);
}

/*

 multipleSignals(Graph g, int v, PriorityQueue& q) sends a signal from 'v'
 to every vertex that is adjacent to 'v' in 'g', excluding those that have
 already received a signal.

 */

void multipleSignals(Graph& g, Vertex& v, PriorityQueue& q)
{
    Edge* adjacents = v.adjList;
    while (adjacents != NULL)
    {
        if (!g.vertices[adjacents->v].signaled)
        {
            double time = g.vertices[adjacents->u].distance;
            singleSignal(adjacents->u, adjacents->v,
                         adjacents->weight + time, q);
        }
        adjacents = adjacents->next;
    }
}

// processEvent(g, q, v) process a single event from
// event queue 'q'.

void processEvent(Graph& g, PriorityQueue& q, Event* e)
{
        Vertex* recip = &(g.vertices[e->receiver]);
        if (trace)
        {
            printf("\nVertex [%i] received a signal. "
                   "Sender: %i, time: %lf\n",
                   e->receiver, e->sender, e->time);
        }
        if (!recip->signaled)
        {
            recip->signaled = true;
            recip->prev = e->sender;
            recip->distance = e->time;

            if (trace)
            {
                printf("\nVertex [%i] updated. "
                       "Previous: %i. Time from start: %lf",
                       e->receiver, e->sender, e->time);
            }
            multipleSignals(g, *recip, q);
        }
    delete e;
}

// getEvent(q) gets an event from priority queue 'q' with the
// least priority and returns the event.

Event* getEvent(PriorityQueue& q)
{
    Event* event;
    double val;
    remove(q, event, val);

    return event;
}

// dijkstarAlgorithm(q, g, start, end) searches for the shortest path
// from vertices 'start' to 'end'.

void dijkstraAlgorithm(PriorityQueue& q, Graph& g, int start, int end)
{
    singleSignal(0, start, 0.0, q);

    while (!g.vertices[end].signaled)
    {
        Event* evnt = getEvent(q);
        double val;
        remove(q, evnt, val);
        processEvent(g, q, evnt);
    }
}

// shortestDistance(g, arg) returns the shortest distance
// from the starting vertex to arg.

double shortestDistance(Graph& g, int arg)
{
    return g.vertices[arg].distance;
}

// showPath(g, strt, end) displays the shortest path
// from 'strt' to 'end' to the standard output.

void showPath(Graph&g, int strt, int end)
{
    if (strt == end)
    {
        printf("%i", strt);
    }
    else
    {
        showPath(g, g.vertices[strt].prev, end);
        printf(" -> ");
        printf("%i", strt);
    }
}
