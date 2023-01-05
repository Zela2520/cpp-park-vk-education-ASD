// Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
// Вариант 1. С помощью алгоритма Прима.
// Вариант 2. С помощью алгоритма Крускала.
// Вариант 3. С помощью алгоритма Борувки.
// Ваш номер варианта прописан в ведомости.
// Формат ввода
// Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
// Следующие m строк содержат описание ребер по одному на строке.
// Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).

// Формат вывода
// Выведите единственное целое число - вес минимального остовного дерева.


#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <cassert>
#include <queue>
#include <memory>
#include <numeric>

using std::cout;
using std::cin;
using std::endl;

struct Edge {
    int from;
    int to;
    int weight;

    Edge(int from = 0, int to = 0, int weight = 0) : from(from), to(to), weight(weight) {}
    bool operator<(const Edge& other) {return this->weight < other.weight;}
    bool operator==(const Edge& other) {return this->weight == other.weight;}
    bool operator!=(const Edge& other) {return this->weight != other.weight;}
};

struct ArcWeightedGraph {
    ArcWeightedGraph(int count);

    void AddEdge(int from, int to, int weight);
    int VerticesCount() const;

    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;
    int MakeKruscalOstovTree();

    void printGraph() {
        std::for_each(edges.begin(), edges.end(),
            [](const Edge& e) {cout << e.from << " -> " << e.to << " [" << e.weight << "]" << endl;});
    }

private:
    int count_vertices;
    std::vector<Edge> edges;
};


ArcWeightedGraph::ArcWeightedGraph (int count) : count_vertices(count) {}

void ArcWeightedGraph::AddEdge(int from, int to, int weight) {
    assert(from > 0 && from <= count_vertices);
    assert(to > 0 && to <= count_vertices);

    edges.push_back(Edge(from, to, weight));
}

int ArcWeightedGraph::VerticesCount() const {
    return count_vertices;
}

std::vector<int> ArcWeightedGraph::GetNextVertices(int vertex) const {
    assert(vertex > 0 && vertex <= VerticesCount());

    std::vector<int> nextVerticies;
    std::for_each(edges.begin(), edges.end(), [&](Edge v) {
        if (v.from == vertex) {
            nextVerticies.push_back(v.to);
        }
    });
    return nextVerticies;
}

std::vector<int> ArcWeightedGraph::GetPrevVertices(int vertex) const {
    assert(vertex > 0 && vertex <= VerticesCount());

    std::vector<int> prevVerticies;
    std::for_each(edges.begin(), edges.end(), [&](Edge v) {
        if (v.to == vertex) {
            prevVerticies.push_back(v.from);
        }
    });

    return prevVerticies;
}

namespace DisjointSetUnion {
    std::vector<int> makeSets(int count_vertices) {
        std::vector<int> sets(count_vertices);
        std::iota(sets.begin(), sets.end(), 1);
        return sets;
    }

    int findSet(int vertex, std::vector<int>& parents) {
        assert(vertex != 0 && vertex - 1 < parents.size());
        if (vertex == parents[vertex - 1]) { // -1 нужен из-за разницы в индексации массива и графа
            return vertex;
        }
        return parents[vertex - 1] = findSet(parents[vertex - 1], parents);
    }

    void unionSet(int first_vertex, int second_vertex, std::vector<int>& parents) {
        parents[DisjointSetUnion::findSet(second_vertex, parents) - 1] =
                DisjointSetUnion::findSet(first_vertex, parents);
    }
} // namespace DisjointSetUnion


std::unique_ptr<ArcWeightedGraph> createGraph() {
    int count_vertices;
    cin >> count_vertices;
    std::unique_ptr<ArcWeightedGraph> Agraph = std::make_unique<ArcWeightedGraph>(count_vertices);

    int edges;
    cin >> edges;

    for (int i = 0; i < edges; ++i) {
        int from;
        int to;
        int weight;
        cin >> from >> to >> weight;
        Agraph->AddEdge(from, to, weight);
        Agraph->AddEdge(to, from, weight);
    }

    return Agraph;
}

int ArcWeightedGraph::MakeKruscalOstovTree() {
    std::sort(edges.begin(), edges.end());

    std::vector<int> parents = DisjointSetUnion::makeSets(VerticesCount());

    int count_sets = parents.size();
    int total_weight = 0;
    std::vector<Edge> edges_ostov;
    for (int i = 0; i < edges.size() && count_sets != 1; ++i) {
        if (DisjointSetUnion::findSet(edges[i].from, parents) != DisjointSetUnion::findSet(edges[i].to, parents)) {
            DisjointSetUnion::unionSet(edges[i].from, edges[i].to, parents);
            edges_ostov.push_back(Edge(edges[i].from, edges[i].to, edges[i].weight));
            edges_ostov.push_back(Edge(edges[i].to, edges[i].from, edges[i].weight));
            total_weight += edges[i].weight;
            --count_sets;
        }
    }

    edges.clear();
    edges = std::move(edges_ostov);

    return total_weight;
}

int main() {
    std::unique_ptr<ArcWeightedGraph> graph = createGraph();
    int total_weight_ostov = graph->MakeKruscalOstovTree();

    std::cout << total_weight_ostov << std::endl;

    return EXIT_SUCCESS;
}
