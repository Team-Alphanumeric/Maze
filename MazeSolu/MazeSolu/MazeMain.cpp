#include <iostream>
#include <limits.h>
#include <vector>
#include <list>
#include <fstream>
#include <queue>
#include "maze.h"
#include <boost/graph/adjacency_list.hpp>
#include <stack>



typedef property<edge_weight_t, int> EdgeProperty;


// Mark all nodes in g as not visited.
void clearVisited(Graph &g)
{
	//loops through all the vertices
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator v = vItrRange.first; v != vItrRange.second; ++v)
	{
		//marks each vertice as not visited		
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
// finding any path using DFS
void findPathDFSRecursive(Graph &g, Graph::vertex_descriptor node, Graph::vertex_descriptor endNode, stack<Graph::vertex_descriptor> &path)
{
	//mark the current node as visited
	g[node].visited = true;		
	//as long as the end node isn't finsihed. a.k.a the path to the end isn't found
	// then keep searching
	if (g[endNode].visited)
	{		
		cout << "End found" << endl; 
		return;
	}
	else
	{
		//push that node to the stack
		path.push(node);
	}
	//iterate through all the adjacnet nodes of the current node to visit them
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange = adjacent_vertices(node, g);
	for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		//if the neigboring node has not been visited then go give them a visit
		//dont' ignore any of your neigbors neigbors!!!
		if (!g[*vItr].visited)
		{
			//recursively call the function so that the function works :|
			findPathDFSRecursive(g, *vItr, endNode, path);			
		}
		
	}
	//only pop of the end hasn't been found because otherwise you
	//will lose the path to your neigbors house which would suck
	//because then you will have to find it again :? That will also
	//require annoying other neigbors because you may have to revisit them
	// and you don't want to revisit the neigbor of the neigbor of the neigbor 
	//down the street.....
	if (!g[endNode].visited)
	{
		path.pop();
	}

}
//fuction to call DFS for any path with less aruguments
void findPathDFSRecursiveCall(Graph &g, Graph::vertex_descriptor endNode, stack<Graph::vertex_descriptor> &pathInit)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);	
	clearVisited(g);
	findPathDFSRecursive(g, *vItrRange.first, endNode, pathInit);
}



int main()
{
	try
	{
		
		ifstream fin;

		// Read the maze from the file.
		string fileName = "maze1.txt";

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
		cout << "Gets here!!" << endl;
		system("pause");
		stack<Graph::vertex_descriptor> pathInit;
		findPathDFSRecursiveCall(g, m.getEnd(), pathInit);
		m.printPath(m.getEnd(), pathInit, g);
		system("pause");
		clearVisited(g);
		setNodeWeights(g, 0);
		clearMarked(g);
		cout << g << endl;		
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