#include <iostream>
#include <limits.h>
#include <vector>
#include <list>
#include <fstream>
#include <queue>
#include "maze.h"
#include <boost/graph/adjacency_list.hpp>



typedef property<edge_weight_t, int> EdgeProperty;


// Mark all nodes in g as not visited.
void clearVisited(Graph &g)
{
	//loops through all the vertices
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator v = vItrRange.first; v != vItrRange.second; ++v)
	{
		//marks each vertice as not visited		g[*v].visited = false;
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