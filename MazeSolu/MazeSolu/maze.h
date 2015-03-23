
// Sample solution for project #5

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <stack>
#include <boost/graph/adjacency_list.hpp>
;
#include "d_except.h"
#include "d_matrix.h"

#define LargeValue 99999999
using namespace boost;
using namespace std;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
	pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;
	bool visited;
	bool marked;
	int weight;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
	int weight;
	bool visited;
	bool marked;
};



class maze
{
public:
	maze(ifstream &fin);
	void print(int, int, int, int);
	bool isLegal(int i, int j);
	void mapMazeToGraph(Graph &g);
	void printPath(Graph::vertex_descriptor end,
		stack<Graph::vertex_descriptor> &s,
		Graph g);
	int numRows(){ return rows; };
	void maze::findIndex(int &i, int &j, const Graph::vertex_descriptor v);
	int numCols(){ return cols; };

private:
	int rows; // number of rows in the maze
	int cols; // number of columns in the maze12 a

	matrix<bool> value; //maze
	matrix<Graph::vertex_descriptor> vMap; // vertices for the maze
	
};

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
	fin >> rows;
	fin >> cols;

	char x;

	value.resize(rows, cols);
	for (int i = 0; i <= rows - 1; i++)
	for (int j = 0; j <= cols - 1; j++)
	{
		fin >> x;
		if (x == 'O')
			value[i][j] = true;
		else
			value[i][j] = false;
	}

	vMap.resize(rows, cols);
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
	cout << endl;

	if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
		throw rangeError("Bad value in maze::print");

	if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
		throw rangeError("Bad value in maze::print");

	for (int i = 0; i <= rows - 1; i++)
	{
		for (int j = 0; j <= cols - 1; j++)
		{
			if (i == goalI && j == goalJ)
				cout << "*";
			else
			if (i == currI && j == currJ)
				cout << "+";
			else
			if (value[i][j])
				cout << " ";
			else
				cout << "X";
		}
		cout << endl;
	}
	cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze, indicating
// whether it is legal to go to cell (i,j).
{
	if (i < 0 || i > rows || j < 0 || j > cols)
		throw rangeError("Bad value in maze::isLegal");

	return value[i][j];
}

// Create a graph g that represents the legal moves in the maze m.
void maze::mapMazeToGraph(Graph &g)
{
	Graph::vertex_descriptor v; // reference to a vertex in the graph
	for (int i = 0; i <= rows - 1; i++)
	for (int j = 0; j <= cols - 1; j++)
	{
		//cout << "Position i and j (" << i << " " << j << ")." << endl;
		//cout << "The value of value[i][j] is " << value[i][j] << endl;
		if (isLegal(i,j))
		{
			// add a vertex to the graph and get vertex descriptor for this position 
			v = add_vertex(g);
			//cout << "Before vMap variables " << endl;
			// add descriptor to the descriptor matrix
			vMap[i][j] = v;	
			g[v].cell.first = i;
			g[v].cell.second = j;
			//cout << "Before if statements" << endl;
			// find indices of connected vertices (i.e. do the left and upper indices contain a vertex)
			if ((i > 0) && (isLegal(i-1,j))) // case upper vertex exists
			{
				add_edge(vMap[i - 1][j], vMap[i][j], g); cout << "Add upper edge " << endl;
			} // add edge between vertices

			if ((j > 0) && (isLegal(i,j-1))) // case upper vertex exists
			{
				add_edge(vMap[i][j - 1], vMap[i][j], g); cout << "Add left edege " << endl;
			} // add edge between vertices
			//cout << "test count " << endl;
		}
	}
}

void maze::printPath(Graph::vertex_descriptor end,
	stack<Graph::vertex_descriptor> &s,
	Graph g)
{	
	// temporary vertex descriptor
	Graph::vertex_descriptor v;	
	
	//cout << "test1 " << endl;
	//system("pause");
	// while the stack is not empty
	while (!s.empty())
	{
		// get the first vertex (.top(), then .pop())
		v = s.top(); s.pop();		
		// call print for this vertex
		print(g[end].cell.first, g[end].cell.second, g[v].cell.first, g[v].cell.second);	
	}
}
// returns the indices of a vertex given a vertex descriptor
void maze::findIndex(int &i, int &j, const Graph::vertex_descriptor v)
{
	//loops through all the verticies
	for (i = 0; i <= rows - 1; i++)
	for (j = 0; j <= cols - 1; j++)
	{
		//if the vertex descriptor finds its corresponding
		//position, then return the location.
		if (vMap[i][j] == v)
		{
			return;
		}
	}
}

ostream &operator<<(ostream &ostr, const Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator v = vItrRange.first; v != vItrRange.second; ++v)
	{
		ostr << "Node descriptor " << *v << " properties: " << endl;
		ostr << "Cell values at ( " << g[*v].cell.first << ", " << g[*v].cell.second << ") " << endl;
		ostr << "Visited? " << ((g[*v].visited) ? "Yes" : "No") << endl;
		ostr << "Weight? " << g[*v].weight << endl;
		
	}
	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);
	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{
		Graph::vertex_descriptor u=target(*eItr, g);
		Graph::vertex_descriptor v=source(*eItr, g);		
		ostr << "Edge descriptor " << *eItr << " properties: " << endl;
		ostr << "There is an edge between (" << g[v].cell.first << " " << g[v].cell.second
			<< ") and (" << g[u].cell.first << " " << g[u].cell.second << "). " << endl;
		ostr << "Visited? " << ((g[*eItr].visited) ? " Yes" : "No") << endl;
		ostr << "Weight?" << g[*eItr].weight << endl;
	}
	return ostr;
}