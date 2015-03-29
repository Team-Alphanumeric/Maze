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
	//only pop off the node if the end hasn't been found because otherwise you
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
// find any path in the graph using DFS with a Stack
void findPathDFSStack(Graph &g, Graph::vertex_descriptor startNode, Graph::vertex_descriptor endNode, stack<Graph::vertex_descriptor> &path)
{	
	//declare variables
	Graph::adjacency_iterator vItr;
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange;
	Graph::vertex_descriptor v;
	//first off, all visited nodes need to be cleared so that
	// there isn't a bug where for some random reason
	// a node is marked as visited, like if another algorithim was run beforehand
	// in the same instace of the code
	clearVisited(g);	
	//push the start node onto the stack
	path.push(startNode);
	//mark the start node as visited because we are currently giving the 
	//start node a visit and it got pushed onto to the stack
	g[startNode].visited = true;	
	//keep running the alorithim until the path is empty which only happens if
	// the end is not found
	while (!path.empty())
	{		
		// find the node currently on the top of the stack
		v  = path.top();
		//find the nodes adjacent to the node currently on the top of the stack
		vItrRange = adjacent_vertices(v, g);		
		//loop through all the adajent nodes of the node currently on the top of the stack
		for (vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
		{
			//If there are unvisted nodes then pay them a visit, it is probably about time you do that.
			//If they are already visited (I mean if you like that person you could pay them another visit 
			//but you ain't got time for that) then the loop will fail and enter into the subsequent if statement
			//following the for loop which pops the currenty node off the stack because there are no more
			//adajcenet nodes and the end is not reached which means that it is a dead end.
			if (!g[*vItr].visited)
			{
				//mark that adjacent node as visited because you just payed that neigbor a visit by seraching for them
				g[*vItr].visited = true;
				//push the node on the stack because it possible that the neigbor can lead you
				//torwards your target!
				path.push(*vItr);		
				//break if you found an adajent node because you must follow that lead before visiting your other neigbors.
				//you will come back assuming that your end goal is not found because if it is, then you ain't giving them
				//a visit because they are of no use to you
				break;
			}			
		}
		//if the for loop contiunes to the case that the iterator equals the end of the node 
		//then none of the adajent nodes to the current top of the stack are unvisited
		//and therefore the node on the top of the stack is currently useless to you 
		//so get rid of it! The end hasn't been found yet and there is no where else to go.
		if (vItr == vItrRange.second)
		{
			path.pop();			
		}
		//WAIT the end has been found!!! Stop seraching and get out of this function!
		if (g[endNode].visited)
		{			
			break;
		}
	}
	//the end hasn't been found and the every node has been visited so
	//someone dun goofed or the maze isn't supposed to have a solution.
	if (!path.empty())
	{
		cout << "What HAPPENED!!!! no path was found" << endl;
	}
}

void findShortestPathDFS(Graph &g, Graph::vertex_descriptor node, Graph::vertex_descriptor endNode, stack<Graph::vertex_descriptor> &bestPath, stack<Graph::vertex_descriptor> &path)
{
	g[node].visited = true;
	path.push(node);
	if (g[endNode].visited)
	{
		cout << "End node visited" << endl;
		cout << "Size of path" << path.size() << endl;
		if (bestPath.size() == 0)
		{
			cout << "Best path found" << endl;
			setStacksEqual<Graph::vertex_descriptor>(path, bestPath);
		}
		else if (path.size() < bestPath.size())
		{
			cout << "Better final path found" << endl;
			setStacksEqual<Graph::vertex_descriptor>(path, bestPath);
		}
		g[node].visited = false;
	}	
	//iterate through all the adjacnet nodes of the current node to visit them
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange = adjacent_vertices(node, g);
	for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		if (!g[*vItr].visited)
		{
			findShortestPathDFS(g, *vItr, endNode, bestPath, path);
		}

	}
	if (!g[endNode].visited)
	{
		g[node].visited = false;
		path.pop();
	}
}




//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  NEED A PRINTING FUNCTION AND TO FIGURE OUT HOW TO CALCULATE THE PATH SKIPPING FOR NOW
void findShortestPathBFS(Graph &g, Graph::vertex_descriptor startNode, Graph::vertex_descriptor endNode, queue<Graph::vertex_descriptor> path)
{
	Graph::adjacency_iterator vItr;
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange;
	Graph::vertex_descriptor v;
	clearVisited(g);
	path.push(startNode);
	g[startNode].visited = true;
	while (!path.empty())
	{
		v = path.front();
		for (vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
		{			
			if (!g[*vItr].visited)
			{
				//mark that adjacent node as visited because you just payed that neigbor a visit by seraching for them
				g[*vItr].visited = true;
				//push the node on the stack because it possible that the neigbor can lead you
				//torwards your target!
				path.push(*vItr);
			}
			if (g[endNode].visited)
			{
				break;
			}
		}
		if (g[endNode].visited)
		{
			break;
		}
		else
		{
			path.pop();
		}
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
		stack<Graph::vertex_descriptor> pathStackInit;
		stack<Graph::vertex_descriptor> pathStackInitBest;
		queue<Graph::vertex_descriptor> pathQueueInit;
		clearVisited(g);
		findShortestPathDFS(g, m.getStart(), m.getEnd(), pathStackInitBest, pathStackInit);
		system("pause");
		m.printPath(m.getEnd(), pathStackInitBest, g);
		system("pause");
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

