#include <iostream>
#include <optional>
#include <vector>
#include <concepts>
#include <type_traits>
#include <string>
#include <algorithm> 
#include <set>
#include <map>

class unidirected_edge { 
    int node_a; 
    int node_b; 
public: 
    unidirected_edge(int u, int v) { 
        if(u<v) { 
            node_a = u; 
            node_b = v; 
        }
        else { 
            node_a = v; 
            node_b = u; //Faccio in modo che node_a sia minore di node_b
        }
    }

    int from() const { 
        return node_a; //from per restituire node_a
    }

    int to() const { 
        return node_b; //to per restituire node_b
    }

    bool operator<(const unidirected_edge& other) const { 
        if(node_a != other.node_a) { 
            return node_a < other.node_a; //Se node_a doverso da other.node_a abbiamo finito, se no controllo il secondo nodo
        }
        return node_b < other.node_b; 
    }

    bool operator==(const unidirected_edge& other) const { 
        if(node_a == other.node_a && node_b == other.node_b) { 
            return true; 
        }
        else { 
            return false; 
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const unidirected_edge& nodo) { 
        os << "(" << nodo.node_a << "," << nodo.node_b << ")"; //Operatore per stampare l'arco
        return os; 
    } 
}; 

class unidirected_graph { 
    std::set<int> nodi; //Nodi del grafo 
    std::set<unidirected_edge> insieme_archi; //Archi del grafo 
    std::map<int, std::set<int>> nodi_adiacenti; //Struttura del grafo, dizionario a cui a ogni nodo sono associati i nodi vicini

    std::vector<unidirected_edge> lista_archi; //Per accedere all'arco con l'indice 
    std::map<unidirected_edge, size_t> edge_indice; //Per accedere all'indice tramite l'arco 

public: 
    unidirected_graph() = default; //Costruttore di default 
    unidirected_graph(const unidirected_graph& other) = default; //Costruttore di copia

    void add_edge(int u, int v) { //Per aggiungere archi
        unidirected_edge e(u,v); 
        if(insieme_archi.find(e) == insieme_archi.end()){ //Controllo se l'arco esiste già: se esiste non faccio nulla (find cerca l'arco, se non lo trova assegna un valore alla fine uguale a .end()) 
           insieme_archi.insert(e); 
           nodi.insert(u); 
           nodi.insert(v); 
           nodi_adiacenti[u].insert(v); 
           nodi_adiacenti[v].insert(u); //Aggiorno i nodi adiacenti: a v associo u e viceversa (dizionario)
           edge_indice[e] = lista_archi.size(); 
           lista_archi.push_back(e); //Aggiungo e alla fine della lista degli archi
        } 
    }

    std::set<int> neighbours(int nodo) const { 
        auto adiacente = nodi_adiacenti.find(nodo); 
        if(adiacente != nodi_adiacenti.end()) { 
            return adiacente->second; 
        }
        return {}; //Se il nodo esiste nella mappa associo i suoi nodi vicini, altrimenti restituisco insieme vuoto 
    }

    const std::set<unidirected_edge>& all_edges() const { 
        return insieme_archi; //Restituisco l'insieme di tutti gli archi 
    }

    const std::set<int>& all_nodes() const { 
        return nodi; //Restituisco insieme di tutti i nodi 
    } 

    size_t edge_number(const unidirected_edge& e) const { 
        return edge_indice.at(e); //Mi dà l'indice dell'arco 
    }

    unidirected_edge edge_at(size_t indice) const {  
        return lista_archi.at(indice); //Mi dà l'arco dato l'indice
    } 

    unidirected_graph operator-(const unidirected_graph& other) const { //Differenza tra grafi 
        unidirected_graph result; 
        for(const auto& e : insieme_archi) { 
            if(other.insieme_archi.find(e) == other.insieme_archi.end()) { //Cerco l'arco, se non lo trovo lo aggiungo al risultato (Grafo differenza)
                result.add_edge(e.from(), e.to()); 
            }
        }
        return result; 
    }; 

}; 

int main() { 
    unidirected_graph g1; 
    g1.add_edge(1,4); 
    g1.add_edge(4,6); 
    g1.add_edge(6,1); //Creazione del grafo con tre nodi e tre archi 

    std::cout << "Il seguente grafo ha tre nodi e tre archi: (1,4), (4,6), (6,1)" << "\n"; 
    //Controllo funzionamento neighbours 
    auto vicini_2 = g1.neighbours(4); 
    std::cout << "\n" << "Test del funzionamento del metodo neighbours: i vicini di 4 sono "; 
    for(int i : vicini_2) { 
        std::cout << i << " "; 
    }
    std::cout << "\n"; 

    //Controllo funzionamento all_edges (restituisce tutti gli archi)
    auto archi_check = g1.all_edges(); 
    std::cout << "\n" << "Test del funzionamento del metodo all_edges: l'insieme degli archi è "; 
    for(auto k : archi_check) { 
        std::cout << k << " "; 
    }
    std::cout << "\n"; 

    //Controllo funzionamento all_nodes (restituisce tutti i nodi) 
    auto nodi_check = g1.all_nodes(); 
    std::cout << "\n" << "Test del funzionamento del metodo all_nodes: l'insieme dei nodi è "; 
    for(auto m : nodi_check) { 
        std::cout << m << " "; 
    }
    std::cout << "\n"; 

    //Controllo funzionamento edge_number (dato un arco, ne restituisce la sua numerazione all'interno del grafo) 
    unidirected_edge e_check_1(4,6); 
    unidirected_edge e_check_2(6,1); 
    unidirected_edge e_check_3(1,4); 
    int indice_check_1 = g1.edge_number(e_check_1); 
    int indice_check_2 = g1.edge_number(e_check_2); 
    int indice_check_3 = g1.edge_number(e_check_3); 
    std::cout << "\n" << "Test del funzionamento del metodo edge_number: " << "\n"; 
    std::cout << "L'indice dell'arco (1,4) all'interno del grafo è " << indice_check_3 << "\n"; 
    std::cout << "L'indice dell'arco (4,6) all'interno del grafo è " << indice_check_1 << "\n"; 
    std::cout << "L'indice dell'arco (6,1) all'interno del grafo è " << indice_check_2 << "\n"; 

    //Controllo funzionamento edge_at (dato un numero d'arco, restituisce il corrispondente oggetto arco all'interno del grafo) 
    auto arco_test_1 = g1.edge_at(indice_check_1); 
    auto arco_test_2 = g1.edge_at(indice_check_2); 
    auto arco_test_3 = g1.edge_at(indice_check_3); 
    std::cout << "\n" << "Test del funzionamento del metodo edge_at: " << "\n"; 
    std::cout << "Nella posizione " << indice_check_3 << " si trova l'arco " << arco_test_3 << "\n"; 
    std::cout << "Nella posizione " << indice_check_1 << " si trova l'arco " << arco_test_1 << "\n"; 
    std::cout << "Nella posizione " << indice_check_2 << " si trova l'arco " << arco_test_2 << "\n"; 

    //Controllo dell'operatore - (differenza tra grafi, G-G' restituisce l'insieme degli archi presenti in G e non presenti in G') 
    unidirected_graph g2; 
    g2.add_edge(1,4); 
    g2.add_edge(4,6); //Due archi in comune con g1, la differenza g1-g2 è (6,1) 
    std::cout << "\n" << "Test del funzionamento della differenza tra grafi: " << "\n"; 
    std::cout << "Grafo g1: (1,4), (4,6), (6,1); Grago g2: (1,4), (4,6). " << "\n" << "g1-g2 = "; 
    unidirected_graph grafo_differenza = g1-g2; 
    for(const auto& t : grafo_differenza.all_edges()) { 
        std::cout << t << " "; 
    }
    std::cout << "\n"; 


    return 0; 



}

