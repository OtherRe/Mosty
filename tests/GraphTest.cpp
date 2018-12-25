#include <Graphs.h>

#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(GRAPHS_TEST_SUITE)
using Edge = std::pair<int, int>;

template <typename T>
void thenSetContains(std::set<T> set, std::initializer_list<T> list)
{
    BOOST_REQUIRE_EQUAL(set.size(), list.size());
    for (auto item : list)
        BOOST_CHECK(set.find(item) != set.end());
}
template <typename T>
void thenVectorContains(std::vector<T> v, std::initializer_list<T> list)
{
    BOOST_REQUIRE_EQUAL(v.size(), list.size());
    for (auto item : list)
        BOOST_CHECK(std::find(v.begin(), v.end(), item) != v.end());
}

template <typename T>
void thenVectorContainsOrdered(std::vector<T> v, std::initializer_list<T> list)
{
    BOOST_REQUIRE_EQUAL(v.size(), list.size());
    auto it1 = v.begin();
    auto it2 = list.begin();
    for (; it1 != v.end(); ++it1, ++it2)
        BOOST_CHECK_EQUAL(*it1, *it2);
}


BOOST_AUTO_TEST_CASE(GivenGraphWithNoVertecies_WhenAddingEdge_ThenItThrows)
{
    auto graph = Graph(0);

    BOOST_CHECK_THROW(graph.addEdge(0, 1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(GivenGraphWithTwoVerteciesAndNoEdges_WhenAddingEdge_DegreeIsOne)
{
    auto graph = Graph(2);

    graph.addEdge(0, 1);

    BOOST_CHECK_EQUAL(graph.getDegree(0), 1);
    BOOST_CHECK_EQUAL(graph.getDegree(1), 1);
}

BOOST_AUTO_TEST_CASE(GivenGraph_WhenAddingEdges_NeighboursAreCorrect)
{
    auto graph = Graph(4);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(1, 3);

    thenSetContains(graph.getNeigbors(0), {1, 2, 3});
    thenSetContains(graph.getNeigbors(1), {0, 3});
    thenSetContains(graph.getNeigbors(2), {0});
    thenSetContains(graph.getNeigbors(3), {0, 1});
}
BOOST_AUTO_TEST_CASE(GivenGraph_WhenEdgesAreAdded_ReturnsCorrectEdgeSet)
{
    auto graph = Graph(4);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);


    thenSetContains(graph.getEdges(), { {0,1}, {0,2}, {0,3}, {1,3}, {2,3} });
}

BOOST_AUTO_TEST_CASE(GivenSimpleGraph_WhenGettingDFSOrderFromOtherSide_ReturnsCorrectOrder)
{
    auto graph = Graph(3);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    thenVectorContainsOrdered(graph.getDFSOrder(2), {2, 1, 0});
}

BOOST_AUTO_TEST_CASE(GivenComplexGraph_WhenGettingDFSOrder_ReturnsCorrectOrder)
{
    auto graph = Graph(6);

    graph.addEdge(0, 1);
    graph.addEdge(0, 4);
    graph.addEdge(1, 4);
    graph.addEdge(2, 4);
    graph.addEdge(4, 5);
    graph.addEdge(3, 4);
    graph.addEdge(3, 5);

    thenVectorContainsOrdered(graph.getDFSOrder(0), {0, 4, 5, 3, 2, 1});
}

BOOST_AUTO_TEST_CASE(GivenPoint_WhenAskingIfIsConnected_ReturnsTrue)
{
    auto graph = Graph(1);


    BOOST_CHECK(graph.isConnected());
}
BOOST_AUTO_TEST_CASE(GivenConnectedGraph_WhenAskingIfIsConnected_ReturnsTrue)
{
    auto graph = Graph(4);

    graph.addEdge(0, 1);
    graph.addEdge(2, 3);

    BOOST_CHECK(!graph.isConnected());
}
BOOST_AUTO_TEST_CASE(GivenDisconnectedGraph_WhenAskingIfConnected_ReturnsFalse)
{
    auto graph = Graph(4);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);

    BOOST_CHECK(graph.isConnected());
}
BOOST_AUTO_TEST_CASE(GivenGraphRectangle_WhenAskingArticulatingPoints_ReturnsNone)
{
    auto graph = Graph(4);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 0);


    BOOST_CHECK_EQUAL(graph.articulationPoints().size(), 0);
}
BOOST_AUTO_TEST_CASE(GivenGraphWithTwoArticulationPoints_WhenAskingForThem_ReturnsCorrectResult)
{
    auto graph = Graph(6);

    graph.addEdge(0, 1);
    graph.addEdge(0, 5);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(4, 3);
    graph.addEdge(4, 2);

    thenSetContains(graph.articulationPoints(), { 0, 1 });
}
BOOST_AUTO_TEST_CASE(GivenRectangleWithTwoDiagonalsAndRemovedSide_WhenAskingArticulationPoints_ReturnsCorrectResult)
{
    auto graph = Graph(5);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);

    thenSetContains(graph.articulationPoints(), {});
}
BOOST_AUTO_TEST_CASE(GivenRectangle_WhenAskingForDfsWithRemovedEdge_ReturnsCorrectResult)
{
    auto graph = Graph(4);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);

    thenVectorContainsOrdered(graph.getDFSOrderWithRemovedEdge(0, {1, 2}), {0});

}
BOOST_AUTO_TEST_CASE(GivenCircle_WhenAskingForDfsWithRemovedEdge_ReturnsCorrectResult)
{
    auto graph = Graph(6);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(5, 4);
    graph.addEdge(0, 5);

    thenVectorContainsOrdered(graph.getDFSOrderWithRemovedEdge(0, {3, 4}), {0, 5, 1, 2});
}



BOOST_AUTO_TEST_SUITE_END()
