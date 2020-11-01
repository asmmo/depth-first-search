#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <concepts>
#include <vector>

using namespace std::string_literals;


template <class T>
class Graph {
public:
    struct Vertex {
        explicit Vertex(T const &data_) : data{data_}, explored{false} {}
        explicit Vertex(T && data_) : data{std::move(data_)}, explored{false} {}

        std::list<std::reference_wrapper<Vertex>> adjacency_list{};
        size_t pre_visit {};
        size_t post_visit {};
        T data;
        bool explored{};
    };

private:
    size_t time{};

    std::list<std::reference_wrapper<Vertex>> graph;
    

    void explore(Vertex  &vertex) {
        if (!vertex.explored) {
            // optional pre-visit procedure
            vertex.pre_visit = ++time;

            std::cout << vertex.data << "\n";
            vertex.explored = true;
            std::for_each(vertex.adjacency_list.begin(), vertex.adjacency_list.end(), [&](auto &son) {
                explore(son);
            });

            // optional post-visit procedure
            vertex.post_visit = ++time;
        }
    }

public:

    void add(Vertex & vertex, Vertex & adjacent_Vertex){

        vertex.adjacency_list.push_back(std::ref(adjacent_Vertex));
        graph.push_back(std::ref(vertex));
    }

    void add(Vertex & vertex){
        graph.push_back(std::ref(vertex));
    }

    template<class ... Vertexes>
    void add(Vertex & vertex, Vertexes& ... adjacent_vertexes)requires std::is_same_v<Vertex, std::common_type_t<Vertexes...>>{
        (add(vertex, std::forward<Vertexes&>(adjacent_vertexes)),...);
    }

    void start_search() {
        std::for_each(graph.begin(), graph.end(), [&](auto &vertex) {
            if (!vertex.get().explored) explore(vertex.get());
        });
    }

    std::vector<std::reference_wrapper<Vertex>> topological_sort(){
        std::string s = ""s;
        Vertex dump{s};
        std::vector<std::reference_wrapper<Vertex>> result (2*graph.size(), dump);

        for(auto const & vertex : graph) result[vertex.get().post_visit - 1] = vertex;
        result.erase(std::remove_if(result.begin(), result.end(), [](auto & el){return el.get().data.empty();}), result.end());
        return {result.rbegin(), result.rend()};
    }

};



/*
 *         A
 *       /|  \                           L
 *      / |   \                          ^
 *     v  V    v                         |
 *     D  C <---B---> G            N     I --> K
 *         \   /\                        |
 *          v v  \                       V
 *           E    v                      M
 *           |    H
 *           |   /
 *           V  v
 *             F
 *             ^
 *             |
 *             O
 *
 */

int main() {

    Graph<std::string>::Vertex A{"A"s}, B{"B"s}, C{"C"s}, D{"D"s}, E{"E"s}, F{"F"s}, G{"G"s}, H{"H"s}, I{"I"s}, J{"J"s}, K{"K"s},
        L{"L"s}, M{"M"s}, N{"N"s}, O{"O"s};

    Graph<std::string> g;
    g.add(A, B, C, D);
    g.add(B, C, E, G, H);
    g.add(C, E);
    g.add(D);
    g.add(E, F);
    g.add(F);
    g.add(G);
    g.add(H, F);
    g.add(I);
    g.add(J, K, L, M);
    g.add(K);
    g.add(L);
    g.add(M);
    g.add(N);
    g.add(O, F);


    g.start_search();
    auto topologically_sorted_dag = g.topological_sort();
    std::cout << "\n\nSorting the DAG\n\n";

    for(auto const & el :topologically_sorted_dag) std::cout << el.get().data << " ";

}
