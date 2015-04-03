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

int getVIndex(Graph::vertex_descriptor v)
{
		
}

bool relax(int u, int v, Graph &g)
{
	
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

