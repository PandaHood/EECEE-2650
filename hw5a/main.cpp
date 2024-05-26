#include <iostream>
#include <fstream>
#include <vector>
#include "maze.h"
using namespace std;

int main() {
    ifstream fin;

    // Read the maze from the file.
    string fileName = "maze3.txt";

    fin.open(fileName.c_str());
    if (!fin)
    {
       cerr << "Cannot open " << fileName << endl;
       exit(1);
    }

    // read file & execute maze operations
    try
    {
        graph g;
        while (fin && fin.peek() != 'Z')
        {
            // load maze & map maze to graph
            maze m(fin);
            m.mapMazeToGraph(g,m);

            // search algorithms
            g.printNodes();
            g.printEdges();
            //m.Recursive(g, 0, g.numNodes()-1);
            cout << "------------------------------------------------------------\n";
            m.NonRecursive(g, 0, g.numNodes()-1);
        }
    }
    catch (indexRangeError &ex)
    {
         cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }

    // close file
    fin.close();

    return 0;
}
