#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <vector>
#include <tuple>
#include <iostream>

class AdjacencyList
{
    std::vector<std::vector<std::tuple<int, double>>> edges;
public:
    AdjacencyList(int size)
    {
        edges.resize(size);
    }

    void add_edge(int source_node, int target_node, double weight)
    {
        auto edge = std::make_tuple(target_node, weight);
        edges[source_node].push_back(edge);
    }
};

#endif /* ADJACENCY_LIST_H */
