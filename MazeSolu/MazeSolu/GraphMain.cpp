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
#include "heapV.h"



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

// set all the vertices as unmarked
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

void insertNodes(heapV<Graph::vertex_descriptor,Graph> &Q, Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItr = vertices(g);
	for(Graph::vertex_iterator v = vItr.first; v != vItr.second; ++v)
	{	Q.minHeapInsert((*v), g); }
	return;
}


// attempts to relax an edge an returns whether the neighboring node's weight changed
// if sucessful, sets the predecessor of the neighbor to the current node
bool relax(Graph::vertex_descriptor currnode, Graph::vertex_descriptor neighnode, Graph &g)
{
	pair<Graph::edge_descriptor, bool> edgChk = edge(neighnode,currnode,g);
	if(edgChk.second) // if the edge exists
	{	
		
		int cwt = g[currnode].weight; // current node's weight
		int nwt = g[neighnode].weight; // neighboring node's weight
		int ewt = g[edgChk.first].weight; // edge weight
		
		if(cwt == LargeValue) {return false;} // relaxation not possible if the current node is at maximum weight
	
		// if going through the current node and edge is less costly than the neighbor's current path . . . 	
		if(cwt + ewt < nwt) 
		{
			g[neighnode].weight = cwt+ewt; // set the neighboring node's weight to using current node/edge
			g[neighnode].pred = currnode; // set the nieghboring node's predecessor to use current node
			return true; // return that the neighboring value was relaxed
		}
	}
	return false; //  return that the neighboring value is unchanged
}

// attempts to relax an edge an returns whether the neighboring node's weight changed
// if sucessful, sets the predecessor of the neighbor to the current node and reset's the
// min priority queue
bool relax(Graph::vertex_descriptor currnode, Graph::vertex_descriptor neighnode, 
Graph &g, heapV<Graph::vertex_descriptor, Graph> &q)
{
	bool changed = relax(currnode,neighnode,g); // attempt relaxation of nodes
	if(changed) { q.minHeapDecreaseKey(q.getIndex(neighnode),g); } // if relaxation is successful, reset priority queue
	return changed; // return whether relaxation occured
}


bool bellmanFord(Graph &g, Graph::vertex_descriptor s)
{
	//set all weights of the nodes in the graph as infinite
	setNodeWeights(g, LargeValue);

	//set the first node's weight to 0
	g[s].weight = 0;
	
	// get a reference to the start/end of the list of edges in the graph
	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);

	// for as many nodes exist in the graph + 1 cycle (for checking)
	for (int i = 0; i <= num_vertices(g); ++i)
	{
		// for all edges, relax every node connected to an edge		
		for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
		{
			//find the current node and the target node for each edge,
			//so that it can be relaxed
			Graph::vertex_descriptor neigh = target(*eItr, g);
			Graph::vertex_descriptor curr = source(*eItr, g);
			
			// relax nodes given by the edge
			bool changed = relax(curr, neigh, g);
			
			// after iteration i = |V| - 1, algorithm should converge:
			// so on iteration i = |V|, check for quiescence
			if(i == num_vertices(g))
			{
				// this will check every vertex twice, but easier than another for loop :)
				if(g[curr].weight == LargeValue) // by now, every vertex should be reachable
					return false;				
				if(changed) // by now, there should be no more relaxation for any edge
					return false; // otherwise, a negative cycle exists
			}
		}
	}
	// If algorithm and final check completes, the algorithm has succeeded
	return true;
}

bool dijkstra(Graph &g, Graph::vertex_descriptor s)
{
	// for the current vertex and neighbor vertex
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
			
			if(!g[nv].visited) // process only unvisited (directed graph / greedy algorithm approach)
			{
				/* 	if neighbor node is not in the queue already, push it onto the queue:
					this implementation ensures that we do not process nodes
					that are not reachable, which would occur if we were to push all 
					nodes onto the priority queue */
			
				if(!nodes.contains(nv)) 
				{	nodes.minHeapInsert(nv,g);	}

				// relax the neighboring node and reset the priority queue
				relax(cv,nv,g,nodes);	
			}		
		}
	}
	
	// when all nodes reachable from the start have been popped, check that 
	// all nodes in the graph have been visited 
	bool checked = true;
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
	{ checked &= g[*vItr].visited; }
	
	// return whether algorithm successfully covers the entire graph
	return checked;
}

string num2string(int i)
{
	string txt="", temp="";
	while(i != 0)
	{
		txt += char((i%10)+48);
		i = i/10;
	}
	temp.resize(txt.size());
	for(i=0;i<txt.size();++i)
	{ temp[i] = txt[txt.size()-1-i]; }
	return temp;
}

int main()
{	
	try
	{
		
		ifstream fin;

		// Read the maze from the file.
		string file = "maze";
		string path = "E:/Users/Thurston Brevett/Documents/Northeastern/Courses/Spring 2015/Algorithms/Maze/maze-files/";
		int mazeInd[] = {15,16,17};
		for(int i=0; i<3; i++)
		{
				
			
			string fileName = path + file + num2string(mazeInd[i]) + ".txt";
			
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
			
			m.print(m.numRows() - 1, m.numCols() - 1, 0, 0,vector<pair<int,int> >());
			system("pause");
			
			Graph g,gj;
			m.mapMazeToGraph(g);
			m.mapMazeToGraph(gj);	

			bool dworked = dijkstra(g,m.getStart());
			if(!dworked) { std::cout << "Warning: Dijkstra's algorithm did not complete entire graph" << endl; }
			if(!g[m.getEnd()].visited) { std::cout << "Dijkstra's algorithm unable to compute path to the end node" << endl; }
			else { m.printPath(m.getEnd(),m.getStart(),g);	}
			
			bool bfworked = bellmanFord(gj,m.getStart());
			if(!bfworked) { std::cout << "Warning: Bellman-Ford algorithm unable to converge on the entire graph" << endl; }
			if(gj[m.getEnd()].weight == LargeValue) { std::cout << "Bellman-Ford algorithm unable to compute path to the end node" << endl; }
			else { m.printPath(m.getEnd(),m.getStart(),gj);	}
			
			

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
	return 0;

}

