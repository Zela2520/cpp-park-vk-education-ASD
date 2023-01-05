#include <iostream>
#include <vector>
#include <cassert>
#include <memory>
#include <functional>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>

using std::cout;
using std::cin;
using std::endl;

#define MAX_COST 214748364

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int cost = 0) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
};


struct ListWeightedGraph : IGraph {
    ListWeightedGraph(int count);

    virtual void AddEdge(int from, int to, int cost) override;
    virtual int VerticesCount() const override;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override;

private:
    std::vector<std::vector<std::pair<int, int>>> adjList;
};

ListWeightedGraph::ListWeightedGraph (int count) {
    adjList.resize(count);
}


void ListWeightedGraph::AddEdge(int from, int to, int cost) {
    if (!(from >= 0 && from < adjList.size())) {
        return;
    }
    if (!(to >= 0 && to < adjList.size())) {
    return;
    }

    adjList[from].emplace_back(to, cost);
}

int ListWeightedGraph::VerticesCount() const {
    return adjList.size();
}

std::vector<std::pair<int, int>> ListWeightedGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjList.size());

    std::vector<std::pair<int, int>> nextVerticies(adjList[vertex].size());
    for (int i = 0; i < adjList[vertex].size(); ++i) {
        nextVerticies[i] = adjList[vertex][i];
    }

    return nextVerticies;
}

size_t algorithmDijkstras(const std::unique_ptr<IGraph>& graph, int begin_point, int end_point) {
    std::vector<int> cost_ways(graph->VerticesCount(), MAX_COST);
    cost_ways[begin_point] = 0;

    std::set<std::pair<int, int>> processed_vertices;
    processed_vertices.emplace(std::make_pair(begin_point, 0));

    while(!processed_vertices.empty()) {
        std::pair<int, int> current_way = *(processed_vertices.begin());
        processed_vertices.erase(current_way);
        for(auto &next_way : graph->GetNextVertices((current_way.first))) {
            if (cost_ways[next_way.first] - next_way.second > cost_ways[current_way.first]) {
                processed_vertices.erase(std::make_pair(next_way.first, cost_ways[next_way.first]));
                cost_ways[next_way.first] = cost_ways[current_way.first] + next_way.second;
                processed_vertices.emplace(std::make_pair(next_way.first, cost_ways[next_way.first]));
            }
        }
    }

    return cost_ways[end_point];
}

std::unique_ptr<IGraph> createGraph(int& a_cost, int& b_cost, int& begin_point, int& end_point) {
    int vertices;
    cin >> a_cost >> b_cost >> vertices >> begin_point >> end_point;


    std::unique_ptr<IGraph> Lgraph = std::make_unique<ListWeightedGraph>(vertices);

    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            Lgraph->AddEdge(i, (i + 1) % vertices, a_cost);
            Lgraph->AddEdge(i, (i*i + 1) % vertices, b_cost);
        }
    }
    return Lgraph;
}


int main () {
    int a_cost;
    int b_cost;

    int begin_point;
    int end_point;

    std::unique_ptr<IGraph> graph = createGraph(a_cost, b_cost, begin_point, end_point);

    cout << algorithmDijkstras(graph, begin_point, end_point);
}
