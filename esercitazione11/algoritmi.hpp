#pragma once

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <type_traits>
#include <string>

template <typename T>
struct fifo {
    std::queue<T> q;
    void put(const T& val) { 
        q.push(val); 
    }
    T get() { 
        T val = q.front(); 
        q.pop(); return val; 
    }
    bool empty() const { 
        return q.empty(); 
    }
};

template <typename T>
struct lifo {
    std::stack<T> s;
    void put(const T& val) { 
        s.push(val); 
    }
    T get() { 
        T val = s.top(); 
        s.pop(); return val; 
    }
    bool empty() const { 
        return s.empty(); 
    }
};


template <typename T>
class undirected_edge { 
    T node_a; 
    T node_b; 
public: 
    undirected_edge(T u, T v) { 
        if(u < v) { 
            node_a = u; 
            node_b = v; 
        } else { 
            node_a = v; 
            node_b = u; 
        }
    }

    T from() const { 
        return node_a; 
    }
    T to() const { 
        return node_b; 
    }

    bool operator<(const undirected_edge& other) const { 
        if(node_a != other.node_a) { 
            return node_a < other.node_a; 
        }
        return node_b < other.node_b; 
    }

    bool operator==(const undirected_edge& other) const { 
        return (node_a == other.node_a && node_b == other.node_b); 
    }

    friend std::ostream& operator<<(std::ostream& os, const undirected_edge& nodo) { 
        os << "(" << nodo.node_a << "," << nodo.node_b << ")"; 
        return os; 
    } 
}; 

template <typename T>
class undirected_graph { 
    std::set<T> nodi;  
    std::set<undirected_edge<T>> insieme_archi;  
    std::map<T, std::set<T>> nodi_adiacenti; 

    std::vector<undirected_edge<T>> lista_archi; 
    std::map<undirected_edge<T>, size_t> edge_indice; 

public: 
    undirected_graph() = default; 
    undirected_graph(const undirected_graph& other) = default; 

    void add_edge(const undirected_edge<T>& e) {
        if(insieme_archi.find(e) == insieme_archi.end()) { 
           insieme_archi.insert(e); 
           nodi.insert(e.from()); 
           nodi.insert(e.to()); 
           nodi_adiacenti[e.from()].insert(e.to()); 
           nodi_adiacenti[e.to()].insert(e.from()); 
           edge_indice[e] = lista_archi.size(); 
           lista_archi.push_back(e); 
        } 
    }

    void add_edge(T u, T v) { 
        add_edge(undirected_edge<T>(u, v));
    }

    std::set<T> neighbours(T nodo) const { 
        auto adiacente = nodi_adiacenti.find(nodo); 
        if(adiacente != nodi_adiacenti.end()) { 
            return adiacente->second; 
        }
        return {}; 
    }

    const std::set<undirected_edge<T>>& all_edges() const { 
        return insieme_archi; 
    }
    const std::set<T>& all_nodes() const { 
        return nodi; 
    } 

    size_t edge_number(const undirected_edge<T>& e) const { 
        return edge_indice.at(e); 
    }
    undirected_edge<T> edge_at(size_t indice) const { 
        return lista_archi.at(indice); 
    } 

    undirected_graph operator-(const undirected_graph& other) const { 
        undirected_graph result; 
        for(const auto& e : insieme_archi) { 
            if(other.insieme_archi.find(e) == other.insieme_archi.end()) { 
                result.add_edge(e); 
            }
        }
        return result; 
    }; 
}; 

template <typename T>
struct nodo_distanza {
    double dist; 
    T nodo;      

    bool operator<(const nodo_distanza& altro) const {
        if (dist != altro.dist) return dist < altro.dist; 
        return nodo < altro.nodo; 
    }
};


template <typename T, typename Container>
undirected_graph<T> visita_grafo(const undirected_graph<T>& grafo, const T& sorgente, Container& c) {
    undirected_graph<T> albero;  
    std::set<T> visitati;       

    c.put(sorgente);          
    visitati.insert(sorgente); 

    while (!c.empty()) {
        T corrente = c.get(); 

        for (const auto& vicino : grafo.neighbours(corrente)) {
            if (visitati.find(vicino) == visitati.end()) {
                visitati.insert(vicino); 
                albero.add_edge(undirected_edge<T>(corrente, vicino)); 
                c.put(vicino); 
            }
        }
    }
    return albero; 
}

template <typename T>
void dfs_ricorsiva_helper(const undirected_graph<T>& grafo, const T& u, std::set<T>& visitati, undirected_graph<T>& albero) {
    for (const auto& v : grafo.neighbours(u)) {
        if (visitati.find(v) == visitati.end()) {
            visitati.insert(v); 
            albero.add_edge(undirected_edge<T>(u, v)); 
            dfs_ricorsiva_helper(grafo, v, visitati, albero); 
        }
    }
}

template <typename T>
undirected_graph<T> dfs_ricorsiva(const undirected_graph<T>& grafo, const T& sorgente) {
    undirected_graph<T> albero; 
    std::set<T> visitati;      
    visitati.insert(sorgente); 
    dfs_ricorsiva_helper(grafo, sorgente, visitati, albero); 
    return albero; 
}

template <typename T>
std::map<T, double> dijkstra(const undirected_graph<T>& rete, 
                             const T& sorgente, 
                             std::map<T, T>& padre, 
                             const std::map<undirected_edge<T>, double>& pesi) {
    
    std::map<T, double> distanze; 
    std::set<nodo_distanza<T>> coda_priorita; 
    
    padre.clear(); 
    distanze[sorgente] = 0.0; 
    
    nodo_distanza<T> nodo_inizio{0.0, sorgente};
    coda_priorita.insert(nodo_inizio); 

    while (!coda_priorita.empty()) {
        auto it = coda_priorita.begin();
        double dist_corrente = it->dist; 
        T u = it->nodo;                  
        coda_priorita.erase(it); 

        if (distanze.find(u) == distanze.end() || dist_corrente <= distanze[u]) {
            for (const auto& v : rete.neighbours(u)) {
                undirected_edge<T> arco(u, v); 
                
                double peso = 1.0;
                auto it_peso = pesi.find(arco);
                if (it_peso != pesi.end()) {
                    peso = it_peso->second;
                }

                bool mai_visitato = (distanze.find(v) == distanze.end());
                
                if (mai_visitato || distanze[u] + peso < distanze[v]) {
                    if (!mai_visitato) {
                        nodo_distanza<T> vecchio_record{distanze[v], v};
                        coda_priorita.erase(vecchio_record); 
                    }
                    
                    distanze[v] = distanze[u] + peso; 
                    padre[v] = u; 
                    
                    nodo_distanza<T> nuovo_record{distanze[v], v};
                    coda_priorita.insert(nuovo_record); 
                }
            }
        }
    }
    return distanze; 
}


using BinaryVector = std::vector<bool>;

struct OrientedEdge {
    size_t edge_id; 
    int sign;       
};

using OrientedCycle = std::vector<OrientedEdge>;

template <typename T>
struct VirtualNode {
    T id;            
    char sheet_sign; 

    bool operator<(const VirtualNode& other) const {
        if (id != other.id) return id < other.id;
        return sheet_sign < other.sheet_sign;
    }
    bool operator==(const VirtualNode& other) const { 
        return id == other.id && sheet_sign == other.sheet_sign; 
    }
    bool operator!=(const VirtualNode& other) const {
        return !(*this == other);
    }
};

inline int dot_product_f2(const BinaryVector& a, const BinaryVector& b) {
    int risultato = 0;
    for (size_t i = 0; i < a.size(); i++) {
        risultato ^= (a[i] && b[i]); 
    }
    return risultato;
}

inline BinaryVector add_f2(const BinaryVector& a, const BinaryVector& b) {
    BinaryVector risultato(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        risultato[i] = a[i] ^ b[i]; 
    }
    return risultato;
}

template <typename T>
bool trova_albero_e_coalbero(const undirected_graph<T>& grafo, 
                             undirected_graph<T>& albero_copertura, 
                             std::vector<undirected_edge<T>>& archi_coalbero,
                             std::vector<BinaryVector>& vettori_supporto) {
    
    auto nodi = grafo.all_nodes();
    if (nodi.empty()) return false;

    albero_copertura = dfs_ricorsiva(grafo, *nodi.begin());

    if (albero_copertura.all_nodes().size() != nodi.size()) {
        return false; 
    }

    undirected_graph<T> grafo_coalbero = grafo - albero_copertura;
    
    for (const auto& arco : grafo_coalbero.all_edges()) {
        archi_coalbero.push_back(arco);
    }

    vettori_supporto.assign(archi_coalbero.size(), BinaryVector(grafo.all_edges().size(), false));
    
    for (size_t i = 0; i < archi_coalbero.size(); i++) {
        vettori_supporto[i][grafo.edge_number(archi_coalbero[i])] = true;
    }
    return true;
}

template <typename T>
std::pair<BinaryVector, OrientedCycle> trova_ciclo_minimo(const undirected_graph<T>& grafo, 
                                                          const std::map<undirected_edge<T>, double>& pesi, 
                                                          const BinaryVector& vettore_firma) {
    
    undirected_graph<VirtualNode<T>> grafo_rivestimento;
    std::map<undirected_edge<VirtualNode<T>>, double> pesi_rivestimento;
    double somma_pesi = 0.0;

    for (const auto& arco : grafo.all_edges()) {
        size_t id_arco = grafo.edge_number(arco);
        
        VirtualNode<T> u_pos{arco.from(), '+'};
        VirtualNode<T> u_neg{arco.from(), '-'};
        VirtualNode<T> v_pos{arco.to(), '+'};
        VirtualNode<T> v_neg{arco.to(), '-'};

        double w = 1.0;
        auto it = pesi.find(arco);
        if (it != pesi.end()) w = it->second;
        somma_pesi += w;

        if (vettore_firma[id_arco] == true) {
            undirected_edge<VirtualNode<T>> e1(u_pos, v_neg);
            undirected_edge<VirtualNode<T>> e2(u_neg, v_pos);
            grafo_rivestimento.add_edge(e1); 
            grafo_rivestimento.add_edge(e2);
            pesi_rivestimento[e1] = w; 
            pesi_rivestimento[e2] = w;
        } else {
            undirected_edge<VirtualNode<T>> e1(u_pos, v_pos);
            undirected_edge<VirtualNode<T>> e2(u_neg, v_neg);
            grafo_rivestimento.add_edge(e1); 
            grafo_rivestimento.add_edge(e2);
            pesi_rivestimento[e1] = w; 
            pesi_rivestimento[e2] = w;
        }
    }

    double peso_minimo_cammino = somma_pesi + 1.0; 
    std::map<VirtualNode<T>, VirtualNode<T>> padri_migliori; 
    VirtualNode<T> inizio_migliore, fine_migliore; 
    bool cammino_trovato = false; 

    for (const auto& nodo : grafo.all_nodes()) { 
        VirtualNode<T> v_pos{nodo, '+'}; 
        VirtualNode<T> v_neg{nodo, '-'}; 
        
        std::map<VirtualNode<T>, VirtualNode<T>> padri_correnti; 
        auto distanze = dijkstra(grafo_rivestimento, v_pos, padri_correnti, pesi_rivestimento); 

        auto it_distanze = distanze.find(v_neg); 
        if (it_distanze != distanze.end() && it_distanze->second < peso_minimo_cammino) { 
            peso_minimo_cammino = it_distanze->second; 
            padri_migliori = padri_correnti;           
            inizio_migliore = v_pos;                   
            fine_migliore = v_neg;                     
            cammino_trovato = true;                    
        } 
    } 

    BinaryVector C(grafo.all_edges().size(), false); 
    OrientedCycle ciclo_orientato;                  

    if (cammino_trovato) { 
        std::vector<VirtualNode<T>> cammino; 
        VirtualNode<T> corrente = fine_migliore; 
        
        while (!(corrente == inizio_migliore)) { 
            cammino.push_back(corrente);             
            corrente = padri_migliori.at(corrente); 
        } 
        cammino.push_back(inizio_migliore); 
        
        std::reverse(cammino.begin(), cammino.end()); 

        for (size_t i = 0; i < cammino.size() - 1; i++) { 
            undirected_edge<T> arco_originale(cammino[i].id, cammino[i+1].id); 
            size_t id_arco = grafo.edge_number(arco_originale);               
            
            C[id_arco] = !C[id_arco]; 

            int segno = -1; 
            if (cammino[i].id < cammino[i+1].id) { 
                segno = 1; 
            } 
            ciclo_orientato.push_back({id_arco, segno}); 
        } 
    } 
    
    return {C, ciclo_orientato}; 
}

template <typename T>
std::vector<OrientedCycle> de_pina(const undirected_graph<T>& grafo, 
                                   const std::map<undirected_edge<T>, double>& pesi, 
                                   std::vector<BinaryVector>& vettori_supporto) {
    
    std::vector<OrientedCycle> base_cicli;
    
    for (size_t i = 0; i < vettori_supporto.size(); i++) {
        auto risultato = trova_ciclo_minimo(grafo, pesi, vettori_supporto[i]);
        BinaryVector C_i = risultato.first;
        
        base_cicli.push_back(risultato.second); 

        for (size_t j = i + 1; j < vettori_supporto.size(); j++) {
            if (dot_product_f2(C_i, vettori_supporto[j]) == 1) {
                vettori_supporto[j] = add_f2(vettori_supporto[j], vettori_supporto[i]);
            }
        }
    }
    return base_cicli;
}

template <typename T>
bool cerca_cammino_albero(const undirected_graph<T>& albero_copertura, 
                          const T& corrente, 
                          const T& target, 
                          std::set<T>& visitati, 
                          std::vector<T>& cammino) {
    
    visitati.insert(corrente); 
    cammino.push_back(corrente); 

    if (corrente == target) return true; 

    for (const auto& vicino : albero_copertura.neighbours(corrente)) {
        if (visitati.find(vicino) == visitati.end()) {
            if (cerca_cammino_albero(albero_copertura, vicino, target, visitati, cammino)) {
                return true; 
            }
        }
    }
    
    cammino.pop_back(); 
    return false;
}

template <typename T>
std::vector<OrientedCycle> cicli_fondamentali_dfs(const undirected_graph<T>& grafo, 
                                                  const undirected_graph<T>& albero_copertura, 
                                                  const std::vector<undirected_edge<T>>& archi_coalbero) {
    std::vector<OrientedCycle> base_cicli;

    for (const auto& corda : archi_coalbero) {
        std::set<T> visitati;
        std::vector<T> cammino;
        
        cerca_cammino_albero(albero_copertura, corda.from(), corda.to(), visitati, cammino);

        OrientedCycle ciclo;
        
        for (size_t i = 0; i < cammino.size() - 1; i++) {
            undirected_edge<T> arco_albero(cammino[i], cammino[i+1]); 
            size_t id_arco = grafo.edge_number(arco_albero); 
            
            int segno = -1;
            if (cammino[i] < cammino[i+1]) {
                segno = 1;
            }
            ciclo.push_back({id_arco, segno}); 
        }

        size_t id_corda = grafo.edge_number(corda);
        
        int segno_corda = -1;
        if (corda.to() < corda.from()) {
            segno_corda = 1;
        }
        ciclo.push_back({id_corda, segno_corda}); 

        base_cicli.push_back(ciclo); 
    }

    return base_cicli;
} 
