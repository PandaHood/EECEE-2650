#ifndef PROJ5A_MAZE_H
#define PROJ5A_MAZE_H

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include <queue>
#include "d_matrix.h"
#include "graph.h"
#include <vector>
#include <algorithm>

using namespace std;


class maze
{
   public:
      maze(ifstream &fin);
      void print(int, int, int, int);
      bool isLegal(int i, int j);

      void setMap(int i, int j, int n);    // i=x, j=y,  n=node id
      int getMap(int i, int j) const;      // i=x, j=y --> get node id
      void mapMazeToGraph(graph &g,maze &m);
      
      void printPath(graph& g, vector<int> ids,int,int);
      vector<int> getCoord(graph& g, int n) const;
      vector<int> getAdjEdges(graph& g, int n);

      void findshortestpath1(graph &g, int, int);
        vector<int> do_bfs(graph &g, int, int);
        void findshortestpath2(graph &g, int,int);
   private:
      int rows; // number of rows in the maze
      int cols; // number of columns in the maze

      matrix<bool> value;
      matrix<int> map;      // Mapping from maze (i,j) values to node index values
};

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n.
{
    map[i][j] = n;
}

int maze ::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
    return map[i][j];
}

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
   fin >> rows;  //cout << rows << endl;  // DEBUG
   fin >> cols;  //cout << cols << endl;  // DEBUG

   char x;

   value.resize(rows,cols);
   for (int i = 0; i <= rows-1; i++)
      for (int j = 0; j <= cols-1; j++)
      {
          fin >> x;
          if (x == 'O')
              value[i][j] = true;
          else
              value[i][j] = false;
      }

   map.resize(rows,cols);
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
   cout << endl;

   if (goalI < 0 || goalI > rows-1 || goalJ < 0 || goalJ > cols-1)
      throw rangeError("Bad value in maze::print");

   if (currI < 0 || currI > rows-1 || currJ < 0 || currJ > cols-1)
      throw rangeError("Bad value in maze::print");

   for (int i = 0; i <= rows-1; i++)
   {
      for (int j = 0; j <= cols-1; j++)
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
// Return the value stored at the (i,j) entry in the maze.
{
   if (i < 0 || i > rows-1 || j < 0 || j > cols-1)
      throw rangeError("Bad value in maze::isLegal");

   return value[i][j];
}

// Create a graph g that represents the legal moves in the maze m.
void maze::mapMazeToGraph(graph &g, maze&m)
{
    int n;
    // add nodes
    for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
            if (isLegal(i,j))
            {
                n = g.addNode(1);    // get node id
                setMap(i, j, n);        // sets node id on graph
            }
            else  // graph nodes that arent present with 0 NodeWeight
            {
                n = g.addNode(0);
                setMap(i,j,n);
            }
		}
	}

    // add edges  (non-directional)
    for (int i = 0; i < value.rows(); i++) {        // i = current row
		for (int j = 0; j < value.cols(); j++) {    // j = current col
            // look for edge ABOVE
            if (i-1 >= 0   &&  value[i-1][j] && value[i][j] && !g.isEdge(getMap(i, j), getMap(i-1, j)))
            {
                int node1 = getMap(i-1, j);  // alt node
                int node2 = getMap(i, j);  // curr node
                g.addEdge(node1, node2, 1);
                g.addEdge(node2, node1, 1);
            }
            else if (i-1 >= 0  &&  !g.isEdge(getMap(i, j), getMap(i-1, j))) //make empty edge between value[i-1][j], (while a real node)
            {
                int node1 = getMap(i-1,j);
                int node2 = getMap(i,j);
                g.addEdge(node1, node2, 0);
                g.addEdge(node2, node1, 0);
            }

            // look for edge RIGHT
            if (j+1 < cols  &&  value[i][j+1] && value[i][j] && !g.isEdge(getMap(i, j), getMap(i, j+1)))
            {
                int node1 = getMap(i, j+1);  // alt node
                int node2 = getMap(i, j);  // curr node
                g.addEdge(node1, node2, 1);
                g.addEdge(node2, node1, 1);
            }
            else if (j+1 < cols  &&  !g.isEdge(getMap(i, j), getMap(i, j+1))) //make empty edge between value[i][j+1], (while a real node)
            {
                int node1 = getMap(i,j+1);
                int node2 = getMap(i,j);
                g.addEdge(node1, node2, 0);
                g.addEdge(node2, node1, 0);
            }

            // look for edge BELOW
            if (i+1 < rows  &&  value[i+1][j] && value[i][j] && !g.isEdge(getMap(i, j), getMap(i+1, j)))
            {
                int node1 = getMap(i+1, j);  // alt node
                int node2 = getMap(i, j);  // curr node
                g.addEdge(node1, node2, 1);
                g.addEdge(node2, node1, 1);
            }
            else if (i+1 < rows  &&  !g.isEdge(getMap(i, j), getMap(i+1, j))) //make empty edge between value[i+1][j], (while a real node)
            {
                int node1 = getMap(i+1,j);
                int node2 = getMap(i,j);
                g.addEdge(node1, node2, 0);
                g.addEdge(node2, node1, 0);
            }

            // look for edge LEFT
            if (j-1 >= 0   &&  value[i][j-1] && value[i][j] && !g.isEdge(getMap(i, j), getMap(i, j-1)))
            {
                int node1 = getMap(i, j-1);  // alt node
                int node2 = getMap(i, j);  // curr node
                g.addEdge(node1, node2, 1);
                g.addEdge(node2, node1, 1);
            }
            else if (j-1 >= 0  &&  !g.isEdge(getMap(i, j), getMap(i, j-1))) //make empty edge between value[i][j-1], (while a real node)
            {
                int node1 = getMap(i,j-1);
                int node2 = getMap(i,j);
                g.addEdge(node1, node2, 0);
                g.addEdge(node2, node1, 0);
            }
        }
	}
}

//The get coordinate function retrieves the proper coord for the maze
vector<int> maze::getCoord(graph& g, int n) const
{
    vector<int> coord;
    for (int i=0;  i<rows;  i++)
        for (int j=0;  j<cols;  j++)
            if (getMap(i,j) == n)
            {
                coord.push_back(i);
                coord.push_back(j);
            }

    if (!coord.empty())
        return coord;
    else if (n == 0)
        {
            coord.push_back(0);
            coord.push_back(0);
            return coord;
        }
    else
        {
            cout << "node " << n << " node not found";
            exit(3);
        }
}

vector<int> maze::getAdjEdges(graph& g, int n)
{
    // get list of all nodes that are adjacent edges  (only track the nodes)
    vector<int> adjEdgeNodes;
    for (int x=0;  x<g.numNodes();  x++)
        if ((x != n)  &&  g.isEdge(x, n)  &&  (1==g.getEdgeWeight(x, n) || 1==g.getEdgeWeight(n,x)))
            adjEdgeNodes.push_back(x);

    return adjEdgeNodes;
}

void maze::printPath(graph& g, vector<int> ids, int iGoal, int jGoal)
{
    int count = 0;
    vector<vector<int>> id_coords;
    for (int x = 0; x < ids.size() - 1; x++)
    {
        id_coords.push_back(getCoord(g, ids[x]));
    }
    // Start tracking the values along path ++ print info
    vector <int> curr_loc = id_coords[0], prev_loc;
    for (int y=0;  y<id_coords.size(); y++)
    {
        prev_loc = curr_loc;
        curr_loc = id_coords[y];

        // is first
        if (y == 0)
            cout << "Starting position at (" << curr_loc[0] << "," << curr_loc[1] << ") via node " << ids[y] << endl;
        // is last
        else if (curr_loc[0] == iGoal && curr_loc[1] == jGoal)
        {
            cout << "(" << prev_loc[0] << "," << prev_loc[1] << ") --> (" << curr_loc[0] << "," << curr_loc[1] << ")"
                 << " from node " << ids[y-1] << " to " << ids[y] << "." << endl;
                 print(iGoal,jGoal,curr_loc[0], curr_loc[1]);
                 break;
        }
        // is UP
        else if ( prev_loc[0] == (curr_loc[0]+1)  &&  prev_loc[1] ==  curr_loc[1]    )
        {
            cout << "(" << prev_loc[0] << "," << prev_loc[1] << ") --> (" << curr_loc[0] << "," << curr_loc[1] << ")"
                 << " from node " << ids[y-1] << " to " << ids[y] << ".  Move Up a node" << endl;
                 print(iGoal,jGoal,curr_loc[0], curr_loc[1]);
        }
        // is RIGHT
        else if ( prev_loc[0] ==  curr_loc[0]     &&  prev_loc[1] == (curr_loc[1]-1) )
        {
            cout << "(" << prev_loc[0] << "," << prev_loc[1] << ") --> (" << curr_loc[0] << "," << curr_loc[1] << ")"
                 << " from node " << ids[y-1] << " to " << ids[y] << ".  Move Right a node" << endl;
                print(iGoal,jGoal,curr_loc[0], curr_loc[1]);
        }
        // is DOWN
        else if ( prev_loc[0] == (curr_loc[0]-1)  &&  prev_loc[1] ==  curr_loc[1]    )
        {
            cout << "(" << prev_loc[0] << "," << prev_loc[1] << ") --> (" << curr_loc[0] << "," << curr_loc[1] << ")"
                 << " from node " << ids[y-1] << " to " << ids[y] << ".  Move Down a node" << endl;
                 print(iGoal,jGoal,curr_loc[0], curr_loc[1]);
        }
        // is LEFT
        else if ( prev_loc[0] ==  curr_loc[0]     &&  prev_loc[1] == (curr_loc[1]+1) )
        {
            cout << "(" << prev_loc[0] << "," << prev_loc[1] << ") --> (" << curr_loc[0] << "," << curr_loc[1] << ")"
                 << " from node " << ids[y-1] << " to " << ids[y] << ".  Move Left a node" << endl;
                 print(iGoal,jGoal,curr_loc[0], curr_loc[1]);
        }
        //else
        else
        {
            cout << "(" << prev_loc[0] << "," << prev_loc[1] << ") --> (" << curr_loc[0] << "," << curr_loc[1] << ")"
                 << " from node " << ids[y-1] << " to " << ids[y] << ".  \n";
                 print(iGoal,jGoal,curr_loc[0], curr_loc[1]);
        }
        count++;
      
    }
    cout << "The maze was traversed in " << ids.size() << " moves!" << endl;
    cout << "Node transit list:  ";
    for (int x=0;  x<count;  x++)
    {
        cout << ids[x];
        if (x != count)
        {
            cout<<", ";
        }
    }

    cout<<ids[ids.size()-1]<<endl;
        

}

void maze::findshortestpath1(graph &g, int node1, int node2)
{
    g.clearMark();
    g.clearVisit();
  
    cout << "Breadth-First-Search algorithm between nodes " << node1 << " and node " << node2 << "!" << endl;
    cout << "Map between designated nodes...  (+: start,  *: end)" << endl;
   
    int iGoal, jGoal, iStart, jStart;
    vector<int> ijTemp = getCoord(g, node1);
    iGoal = ijTemp[0];   jGoal = ijTemp[1];
    ijTemp = getCoord(g, node2);
    iStart = ijTemp[0];  jStart = ijTemp[1];
    print(iGoal, jGoal, iStart, jStart);
    vector<int> bfsNodeOrder;
    bfsNodeOrder = do_bfs(g, node1, node2);
    printPath(g, bfsNodeOrder,iGoal,jGoal);
}


vector<int> maze::do_bfs(graph& g, int v1, int v2) {
    vector<int> shortestPath;
    g.clearMark();

    queue<int> q;
    q.push(v1);

    vector<int> parent(g.numNodes(), -1); // Initialize parent array with -1

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        g.mark(v);

        if (v == v2) {
            // Reconstruct the shortest path
            shortestPath.push_back(v2);
            int current = v2;
            while (parent[current] != -1) {
                shortestPath.push_back(parent[current]);
                current = parent[current];
            }
            reverse(shortestPath.begin(), shortestPath.end());
            return shortestPath;
        }

        vector<int> adjEdgeNodes = getAdjEdges(g, v);

        for (int w : adjEdgeNodes) {
            if (g.getEdgeWeight(v, w) && !g.isMarked(w)) {
                g.mark(w);
                q.push(w);
                parent[w] = v; // Update parent of w
            }
        }
    }
    return shortestPath;
}


void maze::findshortestpath2(graph &g, int v1, int v2)
{
    g.clearMark();
    g.clearVisit();
    int infinity = 99999;
    int numNodes = g.numNodes();
    int iGoal,jGoal;
    vector<int> ijTemp = getCoord(g, v1);
    iGoal = ijTemp[0];   jGoal = ijTemp[1];

    // Initialize distances to infinity
    vector<int> distance(numNodes, infinity);

    // Initialize parent array to track predecessors in the shortest path
    vector<int> parent(numNodes, -1);

    // Set distance from source to itself to 0
    distance[v1] = 0;

    // Priority queue to store nodes based on their current distance from the source
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Push the source node into the priority queue
    pq.push({0, v1});

    while (!pq.empty())
    {
        // Extract the node with the minimum distance from the priority queue
        int currentNode = pq.top().second;
        int currentDistance = pq.top().first;
        pq.pop();

        // If the current node is the end node, terminate the algorithm
        if (currentNode == v2)
            break;

        // Visit all neighboring nodes of the current node
        for (int neighbor = 0; neighbor < numNodes; ++neighbor)
        {
            if (g.isEdge(currentNode, neighbor) && g.getNodeWeight(neighbor) != 0)
            {
                int edgeWeight = g.getEdgeWeight(currentNode, neighbor);

                // Update distance if a shorter path is found
                if (currentDistance + edgeWeight < distance[neighbor])
                {
                    distance[neighbor] = currentDistance + edgeWeight;

                    // Update parent
                    parent[neighbor] = currentNode;

                    // Push the updated node into the priority queue
                    pq.push({distance[neighbor], neighbor});
                }
            }
        }
    }

    
    vector<int> nodelist;
    stack<int> traversal;
    int currentNode = v2;
    while (currentNode != v1)
    {
        traversal.push(currentNode);
        currentNode = parent[currentNode];
    }
   
    nodelist.push_back(v1);
    while (!traversal.empty())
    {
        nodelist.push_back(traversal.top());
        traversal.pop();
    }
    printPath(g, nodelist, iGoal,jGoal);
}



#endif //PROJ5A_MAZE_H
