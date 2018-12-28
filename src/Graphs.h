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

    struct APInfo
    {
        int parent = -1;
        int low = std::numeric_limits<int>::max();
        int disc = 0;
        bool visited = false;
    };

    int pop(std::stack<int> &t)
    {
        auto &i = t.top();
        t.pop();
        return i;
    }

    /**
   * This function creates DFS ordering. Returns a vector of info about
   * each vertex. Time it was discovered, lowest vertex that it is connected in
   * dfs tree and parent of each vertex in DFS
   */
    void createDfsTreeOrdering(int v = 0, int time = 0)
    {
        auto &vertex = vertecies[v];
        info[v].visited = true;
        info[v].disc = info[v].low = time + 1;
        for (auto neighbor : vertex.neighbors)
            if (info[neighbor].visited == false)
            {
                info[neighbor].parent = v;
                createDfsTreeOrdering(neighbor, time + 1);
                info[v].low = std::min(info[v].low, info[neighbor].low);
            }
            else if (info[v].parent != neighbor)
                info[v].low = std::min(info[v].low, info[neighbor].disc);
    }

    size_t numberOfVertecies;
    std::vector<Vertex> vertecies;
    std::vector<APInfo> info;

  public:
    Graph(size_t numberOfVertecies) : vertecies(numberOfVertecies)
    {
        for (size_t i = 0; i < vertecies.size(); ++i)
            vertecies[i].id = i;
        info = std::vector<APInfo>(vertecies.size());
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
        for (const auto &_ : DFS(*this, &vertecies[0]))
        {
            (void)_;
            ++counter;
        }

        return counter == vertecies.size();
    }

    size_t size() const { return vertecies.size(); }

    std::set<int> articulationPoints()
    {
        if (vertecies.size() == 0)
            return {};

        createDfsTreeOrdering();

        std::set<int> result;
        for (size_t i = 1; i < info.size(); ++i)
        {
            if (getDegree(i) == 1)
                continue;

            for (auto neighbor : vertecies[i].neighbors)
            {
                if (info[neighbor].parent == static_cast<int>(i) && info[neighbor].low >= info[i].disc)
                {
                    result.insert(i);
                    break;
                }
            }
        }

        int rootChildren = std::count_if(info.begin(), info.end(),
                                         [](APInfo &v) { return v.parent == 0; });
        if (rootChildren > 1)
            result.insert(0);

        return result;
    }

    std::set<Edge> bgetWideBridges()
    {
        std::set<Edge> result;

        auto artPoints = articulationPoints();

        for (auto artPoint : artPoints)
        {
            auto &neighbors = vertecies[artPoint].neighbors;
            int connectedLeaves =
                std::count_if(neighbors.begin(), neighbors.end(),
                              [&](int n) { return getDegree(n) == 1; });

            if (getDegree(artPoint) >= 3)
                for (auto neighbor : neighbors)
                {
                    if (getDegree(neighbor) != 1 || (getDegree(neighbor) == 1 && connectedLeaves > 1))
                        result.insert({std::min(artPoint, neighbor), std::max(artPoint, neighbor)});
                }
        }

        return result;
    }

    std::set<Edge> getEdges()
    {
        std::set<Edge> result;
        for(auto& v : vertecies)
        {
            for(auto n : v.neighbors)
                result.insert({std::min(n, v.id), std::max(n, v.id)});
        }
        return result;
    }

    std::set<Edge> getWideBidges()
    {
        std::set<Edge> result;
        for(const auto& edge : getEdges())
        {
            auto visited = getDFSOrderWithRemovedEdge(0, edge).size();
            if(visited != size() - 2)
                result.insert(edge);
        }
        return result;
    }

    std::vector<int> getDFSOrder(int vertexId)
    {
        std::vector<int> result;
        for (auto v : DFS(*this, &vertecies[vertexId]))
        {
            result.push_back(v->id);
        }
        return result;
    }

    std::vector<int> getDFSOrderWithRemovedEdge(int vertexId, const Edge& e)
    {
        std::vector<int> result; 
        auto it = DFS(*this, &vertecies[vertexId]).remove_begin(e.first, e.second);
        auto end = DFS(*this, &vertecies[vertexId]).remove_end();
        for (; it != end; ++it)
            result.push_back((*it)->id);
        
        return result;
    }

    class DFS
    {
        Graph &graph;
        Vertex *first;

      public:
        DFS(Graph &graph, Vertex *first) : graph(graph), first(first) {}
        // member typedefs provided through inheriting from std::iterator
        class iterator
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

          public:
            explicit iterator(Graph &graph, Vertex *first)
                : graph(graph), visited(graph.size(), false), current(first)
            {
                if (first)
                    trace.push(first);
            }

            virtual iterator &operator++()
            {
                if (current == nullptr)
                    throw std::out_of_range("DFS has ended.");

                auto vertex = tracePop();

                // visited.insert(vertex);
                visited[vertex->id] = true;
                for (auto neighbor : vertex->neighbors)
                {
                    auto v = &graph[neighbor];
                    if (visited[v->id] == false)
                        trace.push(v);
                }

                while (trace.size() && visited[trace.top()->id] != false)
                {
                    trace.pop();
                }
                current = trace.empty() ? nullptr : trace.top();

                return *this;
            }
            iterator operator++(int)
            {
                iterator retval = *this;
                ++(*this);
                return retval;
            }
            bool operator==(iterator other) const
            {
                bool result = &graph == &other.graph &&
                              trace.size() == other.trace.size() &&
                              current == other.current;
                return result;
            }
            bool operator!=(iterator other) const { return !(*this == other); }
            pointer operator*() const { return current; }
            size_t visitedSize() { return visited.size(); }

            const std::vector<bool> &getVisited() const { return visited; }
        };
        class remove_iterator
            : public iterator
        {
            int u;
            int v;

            bool isRemoved(int id)
            {
                return id == u || id == v;
            }

          public:
            explicit remove_iterator(Graph &graph, Vertex *first, int u, int v)
                : iterator(graph, first), u(u), v(v)
            {
                if(first == nullptr)
                    return;

                if (u == first->id || v == first->id)
                {
                    int i = 1;
                    trace.pop();
                    while (i == u || i == v)
                        ++i;

                    if (static_cast<size_t>(i) == graph.size())
                        current = nullptr;
                    else
                        trace.push(&graph.vertecies[i]);
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
        };
        iterator begin() { return iterator(graph, first); }
        iterator end() { return iterator(graph, nullptr); }
        remove_iterator remove_begin(int u, int v) { return remove_iterator(graph, first, u, v); }
        remove_iterator remove_end() { return remove_iterator(graph, nullptr, 0, 0); }
    };
};

#endif
