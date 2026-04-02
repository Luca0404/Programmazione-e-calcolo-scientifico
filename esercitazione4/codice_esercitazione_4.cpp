#include <iostream>
#include <optional>
#include <vector>
#include <random>
#include <concepts>
#include <type_traits>
#include "randfiller.h"
#include <string>
#include "timecounter.h"
#include <algorithm> 

//3.1 "Verificare che un vettore è ordinato" 
template<typename T> 
bool is_sorted(const std::vector<T>& vec) { 
    if(vec.size()<2) { 
        return true; 
    } 
    for(int i=1; i<vec.size(); i++) { 
        if(vec[i]<vec[i-1]) { 
            return false; 
        } 
    }
    return true; 
}



//5 "Algoritmi di ordinamento" 

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

template<typename T>
void print_vector(const std::vector<T>& v)
{
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
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
    std::cout << "Controllo del funzionamento dei tre algoritmi di ordinamento su 100 vettori riempiti in modo casuale, con numeri interi sia positivi sia negativi, e di dimensione casuale: " << "\n"; 
    randfiller rf; 
    int flag1 = 0;
    int flag2 = 0; 
    int flag3 = 0; 
    for(int k=1; k<100; k++) { 
        int dimensione = rand() % 400 + 1; //Vettori di dimensione casuale tra 1 e 400
        std::vector<int> vi(dimensione); 
        rf.fill(vi, -500, 500); //Valori sia positivi sia negativi, tra -500 e 500 
        std::vector<int> vi1 = vi; 
        std::vector<int> vi2 = vi; 
        bubble_sort(vi); 
        insertion_sort(vi1); 
        selection_sort(vi2); 
         
        if(!is_sorted(vi)) { 
            std::cout << "Errore nel bubble sort" << "\n"; 
            flag1 = 1; 
            return EXIT_FAILURE; 
        } 
        if(!is_sorted(vi1)) { 
            std::cout << "Errore nell'insertion sort" << "\n"; 
            flag2 = 1; 
            return EXIT_FAILURE; 
        } 
        if(!is_sorted(vi2)) { 
            std::cout << "Errore nel selection sort" << "\n"; 
            flag3 = 1; 
            return EXIT_FAILURE; 
        }
    } 
    if(flag1 == 0) { 
        std::cout << "Il bubble sort funziona correttamente" << "\n"; 
    }
    if(flag2 == 0) { 
        std::cout << "L'insertion sort funziona correttamente" << "\n"; 
    }
    if(flag3 == 0) { 
        std::cout << "Il selection sort funziona correttamente" << "\n"; 
    }

    std::cout << "\n" << "Controllo del funzionamento dei tre algoritmi con un vettore riempito con stringhe" << "\n"; 

    std::vector<std::string> vec1 = {"Io", "Mela", "Albero", "Piscina", "Programmazione", "Treno", "Matematica", "Fisica", "Sì", "Notte"}; 
    std::cout << "La stringa iniziale è: " << "\n";
    print_vector(vec1); 
    std::vector<std::string> vec2 = vec1; 
    std::vector<std::string> vec3 = vec1; 
    bubble_sort(vec1); 
    insertion_sort(vec2); 
    selection_sort(vec3); 
    std::cout << "Vettore ordinato con bubble sort: " << "\n"; 
    print_vector(vec1); 
    std::cout << "Vettore ordinato con insertion sort: " << "\n"; 
    print_vector(vec2); 
    std::cout << "Vettore ordinato con selection sort: " << "\n"; 
    print_vector(vec3); 


    //Calcolo il tempo di esecuzione
    std::cout << "\n" << "Calcolo del tempo di esecuzione dei tre algoritmi, per un vettore di dimensione 8192 riempito in modo casuale" << "\n"; 
    timecounter tc; 
    int dim = 8192; 
    std::vector<int> ve_1(dim); 
    rf.fill(ve_1, -500, 500); 
    std::vector<int> ve_2 = ve_1; 
    std::vector<int> ve_3 = ve_1; 
    std::vector<int> ve_4 = ve_1; 

    tc.tic(); 
    bubble_sort(ve_1); 
    double t_bubble_sort = tc.toc(); 
    std::cout << "Tempo di esecuzione per l'algoritmo bubble_sort " << t_bubble_sort << "\n"; 

    tc.tic(); 
    insertion_sort(ve_2); 
    double t_insertion_sort = tc.toc(); 
    std::cout << "Tempo di esecuzione per l'algoritmo insertion_sort " << t_insertion_sort << "\n"; 
    
    tc.tic(); 
    selection_sort(ve_3); 
    double t_selection_sort = tc.toc(); 
    std::cout << "Tempo di esecuzione per l'algoritmo selection_sort " << t_selection_sort << "\n"; 

    std::cout << "Confronto questi risultati con con i tempi della funzione di ordinamento std::sort() fornita dalla libreria standard " << "\n"; 
    tc.tic(); 
    std::sort(ve_4.begin(), ve_4.end()); 
    double t_sort = tc.toc(); 
    std::cout << "Tempo di esecuzione della funzione std::sort() " << t_sort << "\n"; 

    return 0; 

} 