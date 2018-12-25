#include <Graphs.h>

#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(WIDE_BRIDGES_TEST_SUITE)
using Edge = std::pair<int, int>;

template <typename T>
void thenSetContains(std::set<T> set, std::initializer_list<T> list)
{
    BOOST_REQUIRE_EQUAL(set.size(), list.size());
    for (auto item : list)
        BOOST_CHECK(set.find(item) != set.end() || set.find({item.second, item.first}) != set.end());
}


BOOST_AUTO_TEST_CASE(GivenEmptyGraph_WhenGettingWideBridges_ReturnsEmptyVector)
{
    auto graph = Graph(0);

    thenSetContains(graph.getWideBidges(), {});
}

BOOST_AUTO_TEST_CASE(GivenTriangle_WhenGettingWideBridges_ReturnsEmptySet)
{
    auto graph = Graph(3);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 0);

    thenSetContains(graph.getWideBidges(), {});
}

BOOST_AUTO_TEST_CASE(GivenGraphWithWeirdOrdering_WhenGettingWideBridges_ReturnsCorrectResult)
{
    auto graph = Graph(6);

    graph.addEdge(0, 1);
    graph.addEdge(0, 5);
    graph.addEdge(0, 3);
    graph.addEdge(0, 4);
    graph.addEdge(3, 4);
    graph.addEdge(2, 4);

    thenSetContains(graph.getWideBidges(), { {3, 4}, {0, 3}, {0,4}, {0, 5}, {0, 1} });
}

BOOST_AUTO_TEST_CASE(GivenGraphWith3VerteciesAndOnePath_WhenGettingWideBridges_ReturnsCorrectResult)
{
    auto graph = Graph(3);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    thenSetContains(graph.getWideBidges(), {});
}

BOOST_AUTO_TEST_CASE(GivenRectangleWithOneDiagonal_WhenGettingWideBridges_ReturnsCorrectResult)
{
    auto graph = Graph(4);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 0);
    graph.addEdge(0, 2);

    thenSetContains(graph.getWideBidges(), {{0, 2}});
}
BOOST_AUTO_TEST_CASE(GivenComplexGraph_WhenGettingWideBridges_ReturnsCorrectResult)
{
    auto graph = Graph(6);

    graph.addEdge(0, 1);
    graph.addEdge(0, 5);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(4, 3);
    graph.addEdge(4, 2);

    thenSetContains(graph.getWideBidges(), {{0, 1}, {1,2}, {1,3}, {2,3}});
}

BOOST_AUTO_TEST_SUITE_END()

