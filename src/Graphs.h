#ifndef AISDI_GRAPHS_H
#define AISDI_GRAPHS_H

// #include <cstddef>
#include <algorithm>
#include <limits>
#include <set>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

class Graph
{
    using Edge = std::pair<int, int>;

    struct Vertex
    {
        int id;
        size_t degree = 0;
        std::vector<int> neighbors;
    };

    int pop(std::stack<int> &t)
    {
        auto &i = t.top();
        t.pop();
        return i;
    }

    size_t numberOfVertecies;
    std::vector<Vertex> vertecies;

  public:
    Graph(size_t numberOfVertecies) : vertecies(numberOfVertecies)
    {
        for (size_t i = 0; i < vertecies.size(); ++i)
            vertecies[i].id = i;
    }

    void addEdge(size_t first, size_t second)
    {
        if (first >= vertecies.size() || second >= vertecies.size() ||
            first == second)
            throw std::out_of_range("Wrong vetex index");

        vertecies[first].neighbors.push_back(second);
        vertecies[second].neighbors.push_back(first);
    }

    Vertex &operator[](size_t vertexId) { return vertecies.at(vertexId); }

    size_t getDegree(int vertexId)
    {
        return vertecies.at(vertexId).neighbors.size();
    }

    std::vector<int> getNeigbors(size_t vertexId)
    {
        return vertecies.at(vertexId).neighbors;
    }

    bool isConnected()
    {
        if (vertecies.size() == 0)
            return true;

        size_t counter = 0;
        for (const auto &_ : DFS(*this, &vertecies[0], -1, -1))
        {
            (void)_;
            ++counter;
        }

        return counter == vertecies.size();
    }

    size_t size() const { return vertecies.size(); }

    std::set<Edge> getEdges()
    {
        std::set<Edge> result;
        for (auto &v : vertecies)
        {
            for (auto n : v.neighbors)
                result.insert({std::min(n, v.id), std::max(n, v.id)});
        }
        return result;
    }

    std::set<Edge> getWideBidges()
    {
        std::set<Edge> result;
        for (const auto &edge : getEdges())
        {
            auto visited = getDFSOrderWithRemovedEdge(0, edge).size();
            if (visited != size() - 2)
                result.insert(edge);
        }
        return result;
    }

    std::vector<int> getDFSOrder(int vertexId)
    {
        std::vector<int> result;
        for (auto v : DFS(*this, &vertecies[vertexId], -1, -1))
        {
            result.push_back(v->id);
        }
        return result;
    }

    std::vector<int> getDFSOrderWithRemovedEdge(int vertexId, const Edge &e)
    {
        std::vector<int> result;
        for(auto v : DFS(*this, &vertecies[vertexId], e.first, e.second))
            result.push_back(v->id);

        return result;
    }

    class DFS
    {
        Graph &graph;
        Vertex *first;
        int u;
        int v;
      public:
        DFS(Graph &graph, Vertex *first, int u, int v) : graph(graph), first(first), u(u), v(v) {}
        // member typedefs provided through inheriting from std::iterator
        class remove_iterator
            : public std::iterator<std::input_iterator_tag, // iterator_category
                                   Vertex,                  // value_type
                                   Vertex,                  // difference_type
                                   const Vertex *,          // pointer
                                   Vertex                   // reference
                                   >
        {
          protected:
            Graph &graph;
            std::stack<Vertex *> trace;
            std::vector<bool> visited;
            Vertex *current;
            Vertex *tracePop()
            {
                auto v = trace.top();
                trace.pop();
                return v;
            }
            int u;
            int v;

            bool isRemoved(int id)
            {
                return id == u || id == v;
            }

          public:
            explicit remove_iterator(Graph &graph, Vertex *first, int u, int v)
                : graph(graph), visited(graph.size()),current(first), u(u), v(v)
            {
                if (first == nullptr)
                    return;

                trace.push(first);
                if (u == first->id || v == first->id)
                {
                    int i = 1;
                    trace.pop();
                    while (i == u || i == v)
                        ++i;

                    if (static_cast<size_t>(i) == graph.size())
                        current = nullptr;
                    else
                        trace.push(&graph[i]);
                }
            }

            iterator &operator++()
            {
                if (current == nullptr)
                    throw std::out_of_range("DFS has ended.");

                auto vertex = tracePop();

                // visited.insert(vertex);
                visited[vertex->id] = true;
                for (auto neighbor : vertex->neighbors)
                {
                    auto v = &graph[neighbor];
                    if (visited[v->id] == false && !isRemoved(v->id))
                        trace.push(v);
                }

                while (trace.size() && (visited[trace.top()->id] != false || isRemoved(trace.top()->id)))
                {
                    trace.pop();
                }
                current = trace.empty() ? nullptr : trace.top();

                return *this;
            }
            iterator operator++(int)
            {
                remove_iterator retval = *this;
                ++(*this);
                return retval;
            }
            bool operator==(remove_iterator other) const
            {
                bool result = &graph == &other.graph &&
                              trace.size() == other.trace.size() &&
                              current == other.current;
                return result;
            }
            bool operator!=(remove_iterator other) const { return !(*this == other); }
            pointer operator*() const { return current; }
            size_t visitedSize() { return visited.size(); }

            const std::vector<bool> &getVisited() const { return visited; }
        };
        
        remove_iterator begin() { return remove_iterator(graph, first, u, v); }
        remove_iterator end() { return remove_iterator(graph, nullptr, 0, 0); }
    };
};

#endif
