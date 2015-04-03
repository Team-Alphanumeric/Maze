#include <iostream>
#include <limits.h>
#include <vector>
#include <list>
#include <fstream>
#include <queue>
#include "maze.h"
#include <boost/graph/adjacency_list.hpp>
#include <stack>
#include <queue>
#include <list>
#include <string>



typedef property<edge_weight_t, int> EdgeProperty;


// Mark all nodes in g as not visited.
void clearVisited(Graph &g)
{
	//loops through all the vertices
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator v = vItrRange.first; v != vItrRange.second; ++v)
	{
		//marks each vertex as not visited		
		g[*v].visited = false;
	}
}

// Set all node weights to w.
void setNodeWeights(Graph &g, int w)
{
	//loops through all the vertices
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator v = vItrRange.first; v != vItrRange.second; ++v)
	{
		//sets each vertice weight to w
		g[*v].weight = w;
	}
}
template <typename T>
void setStacksEqual(stack <T> firstStack, stack <T> &secondStack)
{
	while (!secondStack.empty())
	{
		secondStack.pop();
	}
	stack <T> intermidate;
	while (!firstStack.empty())
	{
		intermidate.push(firstStack.top());
		firstStack.pop();
	}
	while (!intermidate.empty())
	{
		secondStack.push(intermidate.top());
		intermidate.pop(); 
	}
}
//clear the stack of all data
template <typename T>
void clearStack(stack<T> firstStack)
{
	while (!firstStack.empty())
	{
		firstStack.pop();
	}
}
// set all the vericies as unmarked
void clearMarked(Graph &g)
{
	//loops through all the vertices
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator v = vItrRange.first; v != vItrRange.second; ++v)
	{
		//sets each vertice as not marked
		g[*v].marked = false;
	}
}

// attempts to relax an edge an returns wether the neighboring node's weight changed
bool relax(Graph::vertex_descriptor currnode, Graph::vertex_descriptor neighnode, Graph &g)
{
	pair<Graph::edge_descriptor, bool> edgChk = edge(neighnode,currnode,g);
	if(edgChk.second) // if the edge exists
	{	
		
		int cwt = g[currnode].weight; // current node's weight
		int nwt = g[neighnode].weight; // neighboring node's weight
		int ewt = g[edgChk.first].weight; // edge weight
	
		// if going through the current node is less costly than the neighbor's current path . . . 	
		if(cwt+ewt < nwt) 
		{
			g[neighnode].weight = newWgt; // set the neighboring node's weight to using current node
			g[neighnode].pred = currnode; // set the nieghboring node's predecessor to use current node
			// reset the priority queue after a successful relaxa
			return true; // return that the neighboring value was relaxed
		}
	}
	
	return false; //  return that the neighboring value is unchanged
	
}

void insertNodes(heapV &Q, Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItr = vertices(g);
	for(Graph::vertex_iterator v = vItr.first; v != vItr.second; ++v)
	{	Q.minHeapInsert((*v), g); }
	return;
}


bool dijkstra(Graph &g, Graph::vertex_descriptor s)
{
	// for the current node and neighbor node
	Graph::vertex_descriptor cv, nv;

	// declare the heap that we will use as a priority queue
	heapV<Graph::vertex_descriptor, Graph> nodes;
		
	// unvisit all nodes
	clearVisited(g);
	
	// set all the weights of the nodes in the graph as infinite
	setNodeWeights(g,LargeValue);

	// set the first node's weight to 0
	g[s].weight = 0;
	
	// push first node onto the priority queue
	nodes.minHeapInsert(s,g);

	// for set of adjacent nodes
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vAdj; 

	// process each node until the queue is empty
	while(nodes.size() != 0)
	{
		// extract top node: make current node
		cv = nodes.extractMinHeapMinimum(g);
		
		// visit current node
		g[cv].visited = true;
		
		// find all neighbors of the current node
		vAdj = adjacent_vertices(cv,g);
		
		// relax and push all unvisited neighbors in order of priority
		for(Graph::adjacency_iterator nb = vAdj.first; nb != vAdj.second; ++nb)
		{
			// get a neighboring node
			nv = *nb;
			
			if(!g[nv].visited) // process only unvisited 
			{
				// relax the neighboring node
				relax(cv,nv,g);
				

				/* 	if neighbor node is not in the queue already, push it onto the queue:
					this implementation ensures that we do not process nodes
					that are not reachable, which would occur if we were to push all 
					nodes onto the priority queue */
				bool contains = false;
				for(int i=0; i<nodes.size(); ++i)
				{	contains = contains || 	(nv == nodes.getItem(i));	}
				if(!contains) {	nodes.minHeapInsert(nv,g);	}
			}		
		}
	}
	
	// when all nodes reachable from the start have been popped, 
}

int main()
{	
	try
	{
		
		ifstream fin;

		// Read the maze from the file.
		string file = "maze";
		string path = "E:/Users/Thurston Brevett/Documents/Northeastern/Courses/Spring 2015/Algorithms/Maze/maze-files/";
		for(int i=1; i<=12; i++)
		{
				
			
			string fileName = path + file + (char(i+48)) + ".txt";
			
			cout << "Opening file " << fileName << endl;
			fin.open(fileName.c_str());
			if (!fin)
			{
				cerr << "Cannot open " << fileName << endl;
				system("pause");
				return 1;
			}		
			maze m(fin);
			fin.close();
			
			m.print(m.numRows() - 1, m.numCols() - 1, 0, 0);
			system("pause");
			
			Graph g;
			m.mapMazeToGraph(g);	
			
			stack<Graph::vertex_descriptor> pathStackInit;
			stack<Graph::vertex_descriptor> pathStackInitBest;
			queue<Graph::vertex_descriptor> pathQueueInit;
			clearVisited(g);

			findShortestPathBFS(g, m.getStart(), m.getEnd(), pathQueueInit, pathStackInitBest);		
			m.printPath(m.getEnd(), pathStackInitBest, g);
			system("pause");

			clearVisited(g);
			clearStack(pathStackInitBest);
			clearStack(pathStackInit);

			findShortestPathDFS(g, m.getStart(), m.getEnd(), pathStackInitBest, pathStackInit);
			m.printPath(m.getEnd(), pathStackInitBest, g);
			system("pause");

			clearVisited(g);
			clearStack(pathStackInitBest);
			clearStack(pathStackInit);

			findPathDFSRecursiveCall(g, m.getStart(), m.getEnd(), pathStackInit);
			m.printPath(m.getEnd(), pathStackInit, g);
			system("pause");

			clearVisited(g);
			clearStack(pathStackInitBest);
			clearStack(pathStackInit);

			findPathDFSStack(g, m.getStart(), m.getEnd(), pathStackInit);
			m.printPath(m.getEnd(), pathStackInit, g);
			system("pause");
		}

	}
	catch (rangeError e)
	{
		cout << "Range Error caught " << e.what() << endl;
	}
	catch (...)
	{
		cout << "Unspecified error\n" << endl;
	}
	system("pause");

}
