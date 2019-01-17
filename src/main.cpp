#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include "Graphs.h"

using namespace std;

int main()
{
  int vertecies;
  cin >> vertecies;
  Graph graph(vertecies);

  size_t u, v;
  while (cin)
  {
    cin >> u >> v;
    graph.addEdge(u, v);
  }

  auto wideBridges = graph.getWideBidges();
  for_each(wideBridges.begin(), wideBridges.end(), [](auto e) {
    cout << e.first << " " << e.second << endl;
  });

  return 0;
}
