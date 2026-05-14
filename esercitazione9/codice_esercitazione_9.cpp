#include <iostream>
#include <optional>
#include <vector>
#include <random>
#include <concepts>
#include <type_traits>
#include <string>
#include <algorithm> 
#include <limits>
#include <set>
#include <map>
#include <stack>
#include <queue>

//Dalla esercitazione precedente (struttura grafi)
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

//Implementazione algoritmi 

template<typename T> //Implementazione fifo (first in first out) con operazioni put,get,empty
class fifo { 
private: 
    std::queue<T> q; //std::queue è un container adapter della STL, è come una coda (ptimo elemento inserito è il primo che verrà rimosso)
public: 
    void put(const T& val) { //put inserire un elemento nel contenitore (in fondo alla coda)
        q.push(val); 
    } 
    T get() { //get rimuovere un elemento dal contenitore (l'elemento in testa alla coda)
        T val=q.front(); //con front leggo il primo elemento
        q.pop(); //Rimuovo l'elemento 
        return val; 
    }
    bool empty() const { //empty vedere se il contenitore è vuoto 
        return q.empty(); 
    }
}; 

template<typename T> //lifo (last in first out)
class lifo { 
private: 
    std::stack<T> s;
public: 
    void put(const T& val) { //put inserisce un elemento in cima allo stach
        s.push(val); 
    }
    T get() { //get estrae l'elemento sulla cima dello stack
        T val=s.top(); 
        s.pop(); 
        return val; 
    }
    bool empty() const { 
        return s.empty(); //Vedo se lo stack è vuoto
    }
}; 

//Implementazione graph_visit: prende in input grafo da visitare e nodo sorgente. Prende un parametro template (T) che specifica il tipo di contenitore da usare per effettuare la visita. Restituisce un oggetto di tipo grafo contenente l'albero risultante dalla visita 
template<typename T> 
unidirected_graph graph_visit(const unidirected_graph& G, int nodo_partenza, T& c) { 
    unidirected_graph albero; 
    std::set<int> visitati; //Tengo traccia dei nodi visitati
    c.put(nodo_partenza); 
    visitati.insert(nodo_partenza); 

    while(!c.empty()) { //Continuo fino a che non ci sono più nodi da visitare 
        int u=c.get(); //Estraggo il nodo da analizzare
        for(int v : G.neighbours(u)) { //Guardo i nodi vicini usando metodo neighbours implementato prima
            if(visitati.find(v) == visitati.end()) { //Controllo se il nodo vicino è già stato analizzato: se non è in visitati, procedo
                visitati.insert(v); //Segno il nodo v come visitato
                albero.add_edge(u,v); //Aggiungo l'arco che collega u a v 
                c.put(v); //Metto in c il nuovo nodo scoperto in modo poi da analizzare i suoi vicini 
            }
        }
    }
    return albero; 
}

//recursive_dfs funzione che prende in input grafo da visitare e nodo sorgente e restituisce un oggetto di tipo grafo che è ò'albero risultante dalla visita
void recursive_dfs(const unidirected_graph& G, int u, std::set<int>& visitati, unidirected_graph& albero) { 
    visitati.insert(u); //Metto il nodo u nei visitati
    for(int v : G.neighbours(u)) { 
        if(visitati.find(v) == visitati.end()) { //Procedo se il vicino v non l'ho ancora visitato
            albero.add_edge(u,v); //Aggiungo l'arco
            recursive_dfs(G,v,visitati,albero); //Chiamata ricorsiva, scendo in profondità su questo ramo
        }
    }
}

//Implementazione algoritmo di Dijkstra 

struct Dijstra_1 { 
    std::map<int, double> dist; //Distanza minima dalla sorgente 
    std::map<int, int> pred; //nodo precedente nel cammino minimo 
};

Dijstra_1 Dijkstra(const unidirected_graph& G, const std::map<unidirected_edge, double>& W, int s) { 
    Dijstra_1 r; 
    for(int i : G.all_nodes()) { //for(i=0, i<n, i++)
        r.pred[i] = -1; //Array del predecessore (all'inizio nessun nodo ha un predecessore, li metto con indice nullo)
        r.dist[i] = std::numeric_limits<double>::infinity(); //Array delle distanza (imposto tutte le distanze a infinito, le maggiori possibili)
    }
    r.pred[s] = s; 
    r.dist[s] = 0; 
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> PQ; //Queue che tiene coppe distanza-nodo. Greater perchè il nodo con la distanza minore sia in cima
    for(int i : G.all_nodes()) { 
        PQ.push({r.dist[i], i}); //Inserisco nel grafo tutti i nodi della queue con la loro distanza
    }

    while(!PQ.empty()) { 
        double d = PQ.top().first; 
        int u = PQ.top().second; //Estraggo il nodo u con la distanza minima d
        PQ.pop(); 

        if(d > r.dist[u]) { //Se distanza è maggiore di quella già salvata, significa che ho già un miglior percorso, e posso andare oltre
            continue; 
        }

        for(int w : G.neighbours(u)) { 
            unidirected_edge edge(u, w); //Oggetto arco, per cercare il peso in W (mappa dei pesi)
            if(W.count(edge)) { //W mappa dei pesi
                double weight_u_w = W.at(edge); 
                if(r.dist[w]>r.dist[u]+weight_u_w) { 
                    r.dist[w] = r.dist[u] + weight_u_w; //Aggiorno distanza minima per nodo w
                    r.pred[w] = u; //metto u come predecessore di w
                    PQ.push({r.dist[w], w}); //update priorità di w con nuova distanza
                }
            }
        }
    }
    return r; //Risultato con distanze e predecessori
}

int main() { 
    unidirected_graph G1; 
    G1.add_edge(1,2); 
    G1.add_edge(2,5); 
    G1.add_edge(2,7); 
    G1.add_edge(7,9); 
    G1.add_edge(1,3); 
    G1.add_edge(1,4); 
    G1.add_edge(1,6); 
    G1.add_edge(6,8); //Creazione primo grafo da usare per testare 

    std::cout << "Primo grafo: archi (1,2), (2,5), (2,7), (7,9), (1,3), (1,4), (1,6), (6,8) tutti con peso 1" << "\n";

    std::cout<< "Test BFS" << "\n"; 
    fifo<int> q; 
    unidirected_graph albero1_bfs = graph_visit(G1, 1, q); 
    std::cout << "Gli archi del'albero BFS sono: "; 
    for(auto const& e : albero1_bfs.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test DFS" << "\n"; 
    lifo<int> s; 
    unidirected_graph albero1_dfs = graph_visit(G1, 1, s);
    std::cout << "Gli archi dell'albero DFS sono: "; 
    for(auto const &e : albero1_dfs.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test DFS ricorsiva" << "\n"; 
    std::set<int> visitati; 
    unidirected_graph albero_1_ricorsivo; 
    recursive_dfs(G1, 1, visitati, albero_1_ricorsivo); 
    std::cout << "Gli archi dell'albero DFS risorsiva sono: "; 
    for(auto const& e : albero_1_ricorsivo.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test Dijkstra" << "\n"; 
    //Associo un peso a ogni arco 
    std::map<unidirected_edge, double> pesi; 
    pesi[unidirected_edge(1,2)] = 1.0; 
    pesi[unidirected_edge(2,5)] = 1.0;
    pesi[unidirected_edge(2,7)] = 1.0;
    pesi[unidirected_edge(7,9)] = 1.0;
    pesi[unidirected_edge(1,3)] = 1.0;
    pesi[unidirected_edge(1,4)] = 1.0;
    pesi[unidirected_edge(1,6)] = 1.0;
    pesi[unidirected_edge(6,8)] = 1.0;
    auto result = Dijkstra(G1, pesi, 1); 
    for(int nodo : G1.all_nodes()) { 
        std::cout << "Nodo " << nodo << ", distanza: " << result.dist[nodo] << ", predecessore: " << result.pred[nodo] << "\n"; 
    }

    //Creo un secondo grafo sa usare come test 

    std::cout << "\n" << "Secondo grafo: archi (1,2), (1,3), (1,4), (1,6), (6,7), (5,7), (6,8), (8,9) con pesi, rispettivamente: 1, 1, 1, 2, 3, 4, 2, 1" << "\n"; 

    unidirected_graph G2; 
    G2.add_edge(1,2); 
    G2.add_edge(1,3); 
    G2.add_edge(1,4); 
    G2.add_edge(1,6); 
    G2.add_edge(6,7); 
    G2.add_edge(5,7); 
    G2.add_edge(6,8); 
    G2.add_edge(8,9); 

    std::cout<< "Test BFS" << "\n"; 
    fifo<int> q2; 
    unidirected_graph albero2_bfs = graph_visit(G2, 1, q2); 
    std::cout << "Gli archi del'albero BFS sono: "; 
    for(auto const& e : albero2_bfs.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test DFS" << "\n"; 
    lifo<int> s2; 
    unidirected_graph albero2_dfs = graph_visit(G2, 1, s2);
    std::cout << "Gli archi dell'albero DFS sono: "; 
    for(auto const &e : albero2_dfs.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test DFS ricorsiva" << "\n"; 
    std::set<int> visitati2; 
    unidirected_graph albero_2_ricorsivo; 
    recursive_dfs(G2, 1, visitati2, albero_2_ricorsivo); 
    std::cout << "Gli archi dell'albero DFS risorsiva sono: "; 
    for(auto const& e : albero_2_ricorsivo.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test Dijkstra" << "\n"; 
    //Associo un peso a ogni arco 
    std::map<unidirected_edge, double> pesi2; 
    pesi2[unidirected_edge(1,2)] = 1.0; 
    pesi2[unidirected_edge(1,3)] = 1.0;
    pesi2[unidirected_edge(1,4)] = 1.0;
    pesi2[unidirected_edge(1,6)] = 2.0;
    pesi2[unidirected_edge(6,7)] = 3.0;
    pesi2[unidirected_edge(5,7)] = 4.0;
    pesi2[unidirected_edge(6,8)] = 2.0;
    pesi2[unidirected_edge(8,9)] = 1.0;
    auto result2 = Dijkstra(G2, pesi2, 1); 
    for(int nodo : G2.all_nodes()) { 
        std::cout << "Nodo " << nodo << ", distanza: " << result2.dist[nodo] << ", predecessore: " << result2.pred[nodo] << "\n"; 
    } 

    //Terzo grafo da usare come test 

    unidirected_graph G3; 
    G3.add_edge(1,2); 
    G3.add_edge(1,3); 
    G3.add_edge(1,4); 
    G3.add_edge(1,6); 
    G3.add_edge(2,4); 
    G3.add_edge(2,5); 
    G3.add_edge(2,7); 
    G3.add_edge(3,6); 
    G3.add_edge(4,6); 
    G3.add_edge(4,7); 
    G3.add_edge(5,7); 
    G3.add_edge(6,7); 
    G3.add_edge(6,8); 
    G3.add_edge(7,9); 
    G3.add_edge(8,9); 

    std::cout << "\n" << "Terzo grafo: archi (1,2), (1,3), (1,4), (1,6), (2,4), (2,5), (2,7), (3,6), (4,6), (4,7), (5,7), (6,7), (6,8), (7,9), (8,9) tutti con peso uguale a 1" << "\n";

    std::cout<< "Test BFS" << "\n"; 
    fifo<int> q3; 
    unidirected_graph albero3_bfs = graph_visit(G3, 1, q3); 
    std::cout << "Gli archi del'albero BFS sono: "; 
    for(auto const& e : albero3_bfs.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test DFS" << "\n"; 
    lifo<int> s3; 
    unidirected_graph albero3_dfs = graph_visit(G3, 1, s3);
    std::cout << "Gli archi dell'albero DFS sono: "; 
    for(auto const &e : albero3_dfs.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test DFS ricorsiva" << "\n"; 
    std::set<int> visitati3; 
    unidirected_graph albero_3_ricorsivo; 
    recursive_dfs(G3, 1, visitati3, albero_3_ricorsivo); 
    std::cout << "Gli archi dell'albero DFS risorsiva sono: "; 
    for(auto const& e : albero_3_ricorsivo.all_edges()) { 
        std::cout << e << " "; 
    }
    std::cout << "\n"; 

    std::cout << "Test Dijkstra" << "\n"; 
    //Associo un peso a ogni arco 
    std::map<unidirected_edge, double> pesi3; 
    pesi3[unidirected_edge(1,2)] = 1.0; 
    pesi3[unidirected_edge(1,3)] = 1.0; 
    pesi3[unidirected_edge(1,4)] = 1.0; 
    pesi3[unidirected_edge(1,6)] = 1.0; 
    pesi3[unidirected_edge(2,4)] = 1.0; 
    pesi3[unidirected_edge(2,5)] = 1.0; 
    pesi3[unidirected_edge(2,7)] = 1.0; 
    pesi3[unidirected_edge(3,6)] = 1.0; 
    pesi3[unidirected_edge(4,6)] = 1.0; 
    pesi3[unidirected_edge(4,7)] = 1.0; 
    pesi3[unidirected_edge(5,7)] = 1.0; 
    pesi3[unidirected_edge(6,7)] = 1.0; 
    pesi3[unidirected_edge(6,8)] = 1.0; 
    pesi3[unidirected_edge(7,9)] = 1.0; 
    pesi3[unidirected_edge(8,9)] = 1.0; 
    
    auto result3 = Dijkstra(G3, pesi3, 1); 
    for(int nodo : G3.all_nodes()) { 
        std::cout << "Nodo " << nodo << ", distanza: " << result3.dist[nodo] << ", predecessore: " << result3.pred[nodo] << "\n"; 
    } 






    return 0; 
}

