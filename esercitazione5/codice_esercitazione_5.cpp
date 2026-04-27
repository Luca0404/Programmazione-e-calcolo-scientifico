#include <iostream>
#include <optional>
#include <vector>
#include <random>
#include <concepts>
#include <type_traits>
#include <string>
#include <algorithm> 
#include <limits>
#include "randfiller.h"
#include "timecounter.h"

//Funzione is_sorted per verificare se il vettore è ordinato 
template<typename T>
bool is_sorted(const std::vector<T>& vec) { 
    if(vec.size()<2){ 
        return true; 
    }
    for(int i=1; i<vec.size(); i++) { 
        if(vec[i]<vec[i-1]) { 
            return false;
        }
    }
    return true; 
}

//Algoritmi di ordinamento 
template<typename T> 
void merge(std::vector<T>& A, int p, int q, int r) 
{ 
    int n1 = q-p+1; 
    int n2 = r-q; 
    std::vector<T> L(n1); 
    std::vector<T> R(n2); 
    for(int i=0; i<n1; i++) { 
        L[i] = A[p+i]; 
    } 
    for(int j=0; j<n2; j++) { 
        R[j] = A[q+1+j]; 
    }
    int i=0; 
    int j=0; 
    for(int k=p; k<=r; ++k) { 
        if(i < n1 && (j >= n2 || L[i] <= R[j])) { 
            A[k] = L[i]; 
            i++; 
        } 
        else { 
            A[k] = R[j]; 
            j++; 
        }
    }
} 

template<typename T> 
void merge_sort(std::vector<T>& A, int p, int r) 
{ 
    if(p<r) { 
        int q = (p+r)/2; //Esegue l'arrotondamento per difetto
        merge_sort(A,p,q); 
        merge_sort(A,q+1,r); 
        merge(A,p,q,r); 
    }
} 

template<typename T> 
int partition(std::vector<T>& A, int p, int r) { 
    T x = A[r]; 
    int i=p-1; 
    for(int j=p; j<r; ++j) { 
        if(A[j]<=x) { 
            i++; 
            std::swap(A[i], A[j]);         
        }
    }
    std::swap(A[i+1], A[r]); 
    return i+1; 
} 

template<typename T> 
void quicksort(std::vector<T>& A, int p, int r) { 
    if(p<r) { 
        int q = partition(A,p,r); 
        quicksort(A,p,q-1); 
        quicksort(A,q+1,r); 
    }
}

template<typename T> //Nuovo algoritmo di quicksort che tiene conto del valore di soglia, al di sotto del quale usa l'algoritmo quadratico insertionsort
void quicksort_nuovo(std::vector<T>& A, int p, int r, int soglia) {
    if (p < r) { 
        if ((r-p+1) < soglia) {
            insertion_sort(A); 
        } else {
            int q = partition(A,p,r);
            quicksort_nuovo(A,p,q-1,soglia);
            quicksort_nuovo(A,q+1,r,soglia);
        }
    }
}

template<typename T>
void print_vector(const std::vector<T>& v)
{
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
}


template <typename T> 
void bubble_sort(std::vector<T>& A) 
{ 
    if(A.size()==0) { 
        return; 
    }
    for(int i=0; i<A.size()-1; i++) { 
        for(int j=A.size()-1; j>i; j--) { 
            if(A[j]<A[j-1]) { 
                std::swap(A[j],A[j-1]); 
            }
        }

    }
} 

template <typename T> 
void insertion_sort(std::vector<T>& A) 
{ 
    for(int j=1; j<A.size(); j++) { 
        T key = A[j]; 
        int i = j-1; 
        while(i>=0 && A[i]>key) { 
            A[i+1] = A[i]; 
            i--; 
        }
        A[i+1] = key; 
    }
}

template <typename T> 
void selection_sort(std::vector<T>& A) 
{ 
    for(int i=0; i<A.size()-1; i++) { 
        int min=i; 
        for(int j=i+1; j<A.size(); j++) { 
            if(A[j]<A[min]) { 
                min = j; 
            }
        }
        std::swap(A[i], A[min]); 
    }
}


int main() { 

    //Test per vedere se is_sorted funziona
    std::cout << "Test per vedere se l'algoritmo is_sorted funziona" << "\n"; 
    std::vector<int> vettore_1={3, 5, 7, 9}; 
    bool risultato_1 = is_sorted(vettore_1); 
    if(risultato_1) { 
        std::cout << "Il vettore {3, 5, 7, 9} è ordinato" << "\n"; 

    } 
    else { 
        std::cout << "Il vettore {3, 5, 7, 9} non è ordinato" << "\n"; 
    } 

    std::vector<int> vettore_2={6, 2, 9, 7, 1}; 
    bool risultato_2 = is_sorted(vettore_2); 
    if(risultato_2) { 
        std::cout << "Il vettore {6, 2, 9, 7, 1} è ordinato" << "\n" << "\n"; 
    }
    else { 
        std::cout << "Il vettore {6, 2, 9, 7, 1} non è ordinato" << "\n" << "\n"; 
    } 

    //Test dei tre algoritmi di ordinamento su 100 vettori riempiti in modo casuale 
    std::cout << "Controllo del funzionamento degli algoritmi di ordinamento su 100 vettori riempiti in modo casuale, con numeri interi sia positivi sia negativi, e di dimensione casuale: " << "\n"; 
    randfiller rf; 
    int flag1 = 0;
    int flag2 = 0; 
    for(int k=0; k<100; k++) { 
        int dimensione = rand() % 400 + 1; //Vettori di dimensione casuale tra 1 e 400
        std::vector<int> vi(dimensione); 
        rf.fill(vi, -500, 500); //Valori sia positivi sia negativi, tra -500 e 500 
        std::vector<int> vi1 = vi; 
        std::vector<int> vi2 = vi; 
        merge_sort(vi1, 0, vi1.size()-1); 
        quicksort(vi2, 0, vi2.size()-1); 
         
        if(!is_sorted(vi1)) { 
            std::cout << "Errore nel mergesort" << "\n"; 
            flag1 = 1; 
            return EXIT_FAILURE; 
        } 
        if(!is_sorted(vi2)) { 
            std::cout << "Errore nel quicksort" << "\n"; 
            flag2 = 1; 
            return EXIT_FAILURE; 
        }
    } 
    if(flag1 == 0) { 
        std::cout << "Il mergesort funziona correttamente" << "\n"; 
    }
    if(flag2 == 0) { 
        std::cout << "Il quicksort funziona correttamente" << "\n"; 
    }

    std::cout << "\n" << "Controllo del funzionamento dei tre algoritmi con un vettore riempito con stringhe" << "\n"; 

    std::vector<std::string> vec1 = {"Io", "Mela", "Albero", "Piscina", "Programmazione", "Treno", "Matematica", "Fisica", "Sì", "Notte"}; 
    std::cout << "La stringa iniziale è: " << "\n";
    print_vector(vec1); 
    std::vector<std::string> vec2 = vec1; 
    std::vector<std::string> vec3 = vec1; 
    merge_sort(vec2, 0, vec2.size()-1); 
    quicksort(vec3, 0, vec3.size()-1); 
    std::cout << "Vettore ordinato con mergesort: " << "\n"; 
    print_vector(vec2); 
    std::cout << "Vettore ordinato con quicksort: " << "\n"; 
    print_vector(vec3); 

    timecounter tc; 
    const int num_vettori = 100; 
    const int dim_vettori = 50; 
    std::cout << "\n" << "Calcolo del tempo medio per l'ordinamento di 100 vettori casuali di dimensione 50" << "\n"; 

    std::vector<std::vector<int>> insieme(num_vettori, std::vector<int>(dim_vettori)); 
    for(auto& v : insieme) { 
        rf.fill(v, -1000, 1000); 
    } 

    std::vector<std::vector<int>> insieme_1 = insieme; 
    tc.tic(); 
    for(int i=0; i<num_vettori; i++) { 
        quicksort(insieme_1[i], 0, dim_vettori-1); 
    }
    double tempo_1 = tc.toc(); 
    double tempo_quicksort = tempo_1/num_vettori; 
    std::cout << "Il tempo medio del quicksort è " << tempo_quicksort << "\n"; 

    std::vector<std::vector<int>> insieme_2 = insieme; 
    tc.tic(); 
    for(int i=0; i<num_vettori; i++) { 
        merge_sort(insieme_2[i], 0, dim_vettori-1); 
    } 
    double tempo_2 = tc.toc(); 
    double tempo_mergesort = tempo_2/num_vettori; 
    std::cout << "Il tempo medio del mergesort è " << tempo_mergesort << "\n"; 

    std::vector<std::vector<int>> insieme_3 = insieme; 
    tc.tic(); 
    for(int i=0; i<num_vettori; i++) { 
        bubble_sort(insieme_3[i]); 
    } 
    double tempo_3 = tc.toc(); 
    double tempo_bubblesort = tempo_3/num_vettori; 
    std::cout << "Il tempo medio del bubblesort è " << tempo_bubblesort << "\n"; 

    std::vector<std::vector<int>> insieme_4 = insieme; 
    tc.tic(); 
    for(int i=0; i<num_vettori; i++) { 
        insertion_sort(insieme_4[i]); 
    }
    double tempo_4 = tc.toc(); 
    double tempo_insertionsort = tempo_4/num_vettori; 
    std::cout << "Il tempo medio dell'insertionsort è " << tempo_insertionsort << "\n"; 

    std::vector<std::vector<int>> insieme_5 = insieme; 
    tc.tic(); 
    for(int i=0; i<num_vettori; i++) { 
        selection_sort(insieme_5[i]); 
    } 
    double tempo_5 = tc.toc(); 
    double tempo_selectionsort = tempo_5/num_vettori; 
    std::cout << "Il tempo medio del selectionsort è " << tempo_selectionsort << "\n"; 

    //Confronto con std::sort 
    std::vector<std::vector<int>> insieme_6 = insieme; 
    tc.tic(); 
    for(int i=0; i<num_vettori; i++) { 
        std::sort(insieme_6[i].begin(), insieme_6[i].end()); 
    } 
    double tempo_6 = tc.toc(); 
    double tempo_sort = tempo_6/num_vettori; 
    std::cout << "Il tempo medio per std::sort è " << tempo_sort << "\n"; 


    //Calcolo del valore di soglia 
    std::cout << "\n" << "Calcolo del valore di soglia" << "\n"; 
    for(int dim=2; dim<= 50; ++dim) { 
        std::vector<std::vector<int>> ins(num_vettori, std::vector<int>(dim)); 
        for(int i=0; i<num_vettori; ++i) { 
            rf.fill(ins[i], -1000, 1000); 
        }

        std::vector<std::vector<int>> ins_1 = ins; 
        tc.tic(); 
        for(int i=0; i<num_vettori; i++) { 
            quicksort(ins_1[i], 0, dim-1); 
        }
        double t_1 = tc.toc()/num_vettori; 

        std::vector<std::vector<int>> ins_2 = ins; 
        tc.tic(); 
        for(int i=0; i<num_vettori; i++) { 
            insertion_sort(ins_2[i]); 
        } 
        double t_2 = tc.toc()/num_vettori; 

        if(t_1<t_2) { 
            int soglia = dim; 
            std::cout << "Il valore di soglia è " << soglia << "\n";
            std::cout << "Al di sotto di questa dimensione del vettore da ordinare conviene usare l'algoritmo quadratico insertionsort, mentre al di sopra di questo valore conviene usare l'algoritmo logaritmico quiksort" << "\n"; 
            break; 
        }

    }


    return 0; 

}