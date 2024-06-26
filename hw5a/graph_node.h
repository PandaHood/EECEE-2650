

#include <iostream>
#include <fstream>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <stack>
using namespace std;

typedef int NodeWeight;


class node
{
    public:
      node();
      node(const node &);
      node &operator=(const node &);

      void setId(int i);
      int getId() const;

      void setWeight(NodeWeight);
      NodeWeight getWeight() const;

      void setNode(int, NodeWeight, bool, bool);

      void mark();
      void unMark();
      bool isMarked() const;

      void visit();
      void unVisit();
      bool isVisited() const;

   private:
      int id;
      NodeWeight weight;
      bool marked;
      bool visited;
};

node::node()
// Constructor, creates a new, uninitialized node. Id is initialized
// elsewhere (by the graph constructor).
{
   unMark();
   unVisit();
   setWeight(0);
}

node::node(const node &n)
// Copy constructor
{
   setNode(n.getId(), n.getWeight(), n.isMarked(), n.isVisited());
}

node &node::operator=(const node &n)
// Overloaded assignment operator.
{
   setNode(n.getId(), n.getWeight(), n.isMarked(), n.isVisited());
   return *this;
}

NodeWeight node::getWeight() const
// Return node's weight
{
   return weight;
}

void node::setWeight(NodeWeight w)
// Set node's weight to w.
{
   weight = w;
}

void node::setId(int i)
// Set node's id to i.  Throws an exception if i < 0.
{
   if (i < 0)
      throw rangeError("Bad value in node::setId");

   id = i;
}

int node::getId() const
// Return node's id
{
   return id;
}

void node::setNode(int id, NodeWeight w = 0, bool m = false, bool v = false)
// Initialize a node;
{
   setId(id);
   setWeight(w);

   if (m)
      mark();
   else
      unMark();

   if (v)
      visit();
   else
      unVisit();
}

void node::mark()
   // Mark node.
{
   marked = true;
}

void node::unMark()
   // Un-mark node.
{
   marked = false;
}

bool node::isMarked() const
   // Return true if node is marked, and false otherwise.
{
   return marked;
}

void node::visit()
   // Set visited to true;
{
    visited = true;
}

void node::unVisit()
   // Set visited to false;
{
   visited = false;
}

bool node::isVisited() const
   // Return true if node is visited, and false otherwise.
{
   return visited;
}


ostream &operator<<(ostream &ostr, const node &n)
{
   ostr << "node: " << n.getId() << " weight: " << n.getWeight()
        << " visited: " << n.isVisited() << " marked " << n.isMarked() << endl;

   return ostr;
}


#ifndef PROJ5A_GRAPH_NODE_H
#define PROJ5A_GRAPH_NODE_H

#endif //PROJ5A_GRAPH_NODE_H
