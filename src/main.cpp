#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include "Graphs.h"

using namespace std;

int main()
{
auto graph = Graph(3);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

  graph.getDFSOrder(0);
  return 0;
}
