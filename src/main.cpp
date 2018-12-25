#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include "Graphs.h"

using namespace std;

int main(int argc, char **argv)
{
  auto graph = Graph(6);

 graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(5, 4);
    graph.addEdge(0, 5);

  graph.getDFSOrderWithRemovedEdge(0, {3, 4});
  return 0;
}
