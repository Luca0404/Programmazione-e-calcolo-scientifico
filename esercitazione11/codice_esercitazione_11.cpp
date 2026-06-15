#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <type_traits>
#include <string>
#include "algoritmi.hpp"

int main() { 

    
    std::cout << "Grafo: archi (1,2), (1,3), (1,4), (1,6), (2,4), (2,5), (2,7), (3,6), (4,6), (4,7), (5,7), (6,7), (6,8), (7,9), (8,9) tutti con peso 1\n";
    undirected_graph<int> G3; 
    G3.add_edge(1,2); G3.add_edge(1,3); G3.add_edge(1,4); G3.add_edge(1,6); 
    G3.add_edge(2,4); G3.add_edge(2,5); G3.add_edge(2,7); G3.add_edge(3,6); 
    G3.add_edge(4,6); G3.add_edge(4,7); G3.add_edge(5,7); G3.add_edge(6,7); 
    G3.add_edge(6,8); G3.add_edge(7,9); G3.add_edge(8,9); 

    std::cout << "Test BFS\n"; 
    fifo<int> q3; 
    undirected_graph<int> albero3_bfs = visita_grafo(G3, 1, q3); 
    std::cout << "Gli archi dell'albero BFS sono: "; 
    for(auto const& e : albero3_bfs.all_edges()) { std::cout << e << " "; }
    std::cout << "\n"; 

    std::cout << "Test DFS\n"; 
    lifo<int> s3; 
    undirected_graph<int> albero3_dfs = visita_grafo(G3, 1, s3);
    std::cout << "Gli archi dell'albero DFS sono: "; 
    for(auto const &e : albero3_dfs.all_edges()) { std::cout << e << " "; }
    std::cout << "\n"; 

    std::cout << "Test DFS ricorsiva\n"; 
    undirected_graph<int> albero_3_ricorsivo = dfs_ricorsiva(G3, 1); 
    std::cout << "Gli archi dell'albero DFS ricorsiva sono: "; 
    for(auto const& e : albero_3_ricorsivo.all_edges()) { std::cout << e << " "; }
    std::cout << "\n"; 

    std::cout << "Test Dijkstra\n"; 
    std::map<undirected_edge<int>, double> pesi3; 
    pesi3[undirected_edge<int>(1,2)] = 1.0; pesi3[undirected_edge<int>(1,3)] = 1.0; 
    pesi3[undirected_edge<int>(1,4)] = 1.0; pesi3[undirected_edge<int>(1,6)] = 1.0; 
    pesi3[undirected_edge<int>(2,4)] = 1.0; pesi3[undirected_edge<int>(2,5)] = 1.0; 
    pesi3[undirected_edge<int>(2,7)] = 1.0; pesi3[undirected_edge<int>(3,6)] = 1.0; 
    pesi3[undirected_edge<int>(4,6)] = 1.0; pesi3[undirected_edge<int>(4,7)] = 1.0; 
    pesi3[undirected_edge<int>(5,7)] = 1.0; pesi3[undirected_edge<int>(6,7)] = 1.0; 
    pesi3[undirected_edge<int>(6,8)] = 1.0; pesi3[undirected_edge<int>(7,9)] = 1.0; 
    pesi3[undirected_edge<int>(8,9)] = 1.0; 
    
    std::map<int, int> padri3;
    auto distanze3 = dijkstra(G3, 1, padri3, pesi3); 
    for(int nodo : G3.all_nodes()) { 
        std::cout << "Nodo " << nodo << ", distanza: " << distanze3[nodo];
        if (padri3.count(nodo)) std::cout << ", predecessore: " << padri3[nodo];
        std::cout << "\n"; 
    } 

    std::cout << "\nTest ricerca albero e coalbero\n";
    undirected_graph<int> albero_copertura3; 
    std::vector<undirected_edge<int>> archi_coalbero3;
    std::vector<BinaryVector> vettori_supporto3;

    bool connesso = trova_albero_e_coalbero(G3, albero_copertura3, archi_coalbero3, vettori_supporto3);

    if (!connesso) {
        std::cout << "Il grafo G3 non e' connesso, non è possibile calcolare la base dei cicli.\n";
    } else {
        std::cout << "Albero di copertura trovato\n";
        std::cout << "Numero di archi nel coalbero: " << archi_coalbero3.size() << "\n";

        std::cout << "\nTest cicli fondamentali con dfs\n";
        auto cicli_fondamentali = cicli_fondamentali_dfs(G3, albero_copertura3, archi_coalbero3);
        
        for (size_t i = 0; i < cicli_fondamentali.size(); ++i) {
            std::cout << "Ciclo fondamentale: ";
            for (const auto& arco_orientato : cicli_fondamentali[i]) {
                std::cout << arco_orientato.edge_id << " con segno " << arco_orientato.sign << "; ";
            } 
            std::cout << "\n";
        }

        std::cout << "\nTest cicli minimi (De Pina)\n";
        auto base_minima = de_pina(G3, pesi3, vettori_supporto3);
        
        for (size_t i = 0; i < base_minima.size(); ++i) {
            std::cout << "Ciclo minimo: ";
            for (const auto& arco_orientato : base_minima[i]) {
                std::cout << arco_orientato.edge_id << " con segno " << arco_orientato.sign << "; ";
            }
            std::cout << "\n";
        }
    }

    return 0; 
}

