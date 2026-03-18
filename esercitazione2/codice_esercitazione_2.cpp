# include <iostream>
#include <fstream> 
#include <string> 
#include <cmath>
int main()
{
  //Prima parte dell'esercitazione  
  std:: cout << "Prima parte dell'esercitazione"; 
  double ad[4] = {0.0, 1.1, 2.2, 3.3};
  float af[8] = {0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
  int ai[3] = {0, 1, 2};

  int x = 1;
  float y = 1.1;

  std:: cout << "Stampa degli indirizzi degli elementi dell'array ad: \n";
  std:: cout << &ad[0] << "\n" << &ad[1] << "\n" << &ad[2] << "\n" << &ad[3] <<"\n"; 
  //Gli elementi dell'array ad si trovano a distanza di 8 byte gli uni dagli altri (un elemento di tipo byte occupa 8 byte) 

  std:: cout << "Stampa degli indirizzi degli elementi dell'array af: \n";
  int i=0; 
  while(i<=7) {
    std::cout << &af[i] << "\n"; 
    i++; 
  }
  //Gli elementi dell'array af si trovano a distanza di 4 byte gli uni dagli altri (un elemento di tipo float occupa 4 byte) 

  std:: cout << "Stampa degli indirizzi degli elementi dell'array ai: \n"; 
  std:: cout << &ai[0] << "\n" << &ai[1] << "\n" << &ai[2] << "\n"; 
  //Gli elementi dell'array ai si trovano a distanza di 8 byte gli uni dagli altri (un elementi di tipo int occupa 4 byte)

  std:: cout << "Stampa dell'indirizzo della variabile scalare x: \n" << &x << "\n"; 
  std:: cout << "Stampa dell'indirizzo della variabile scalare y: \n" << &y << "\n"; 
  //Le variabili scalari di tipo int e float occupano 4 byte: leggendo l'ultima parte dell'indirizzo di memoria (scritto in sistema esadecimale): 
  //per x "d0" (208 in sistema decimale) 
  //per y "cc" (204 in sistema decimale) 
  //Dunque le viariabili x e y sono state memorizzate "vicine", senza lasciare spazi (tra gli array invece sono stati lasciati dei "buchi" nella memoria, cioè sono stati memorizzati in parti diverse all'interno della memoria)

  (&y)[1] = 0; 
  //La variabile y è stata memorizzata prima della variabile x, e ponendo l'indice 1 viene fatto un salto nella memoria di 4 byte fino alla posizione occupata dalla variabile x
  //x viene dunque sovrascritta e posta uguale a 0 
  //Il programma dunque stampa infine 0

  std:: cout << x << "\n";


  //Seconda parte dell'esercitazione
  std:: cout << "\n";
  std:: cout << "Seconda parte dell'esercitazione"; 
  std:: cout << "\n"; 
  static const int N = 10; 
  double arr[N] = {3.2, 5.5, 7.8, 3.2, 5.9, 3.2, 8.6, 4.2, 2.4, 6.2}; 
  double minimo = arr[0]; 
  double massimo = arr[0]; 
  int j = 1; 
  while(j<=9) {
    minimo = std::min(minimo, arr[j]); 
    massimo = std::max(massimo, arr[j]); 
    j++; 
  }
  std:: cout << "Il minimo è: " << minimo << "\n"; 
  std:: cout << "Il massimo è: " << massimo << "\n"; 

  int z = 0; 
  double somma = 0; 
  while(z<=9) {
    somma = somma + arr[z]; 
    z++; 
  }
  float media = somma/N; 
  std:: cout << "La media è: " << media << "\n"; 

  //Calcolo della deviazione standard 
  int h = 0; 
  double somma_dev = 0; 
  while(h<=9) {
    somma_dev = somma_dev + ((arr[h]-media)*(arr[h]-media)); 
    h++; 
  }
  double varianza = somma_dev/N; 
  double dev_standard = std::sqrt(varianza); 
  std:: cout << "La deviazione standard è: " << dev_standard << "\n"; 

  return 0;
}
