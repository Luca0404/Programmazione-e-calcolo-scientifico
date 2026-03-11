#include <iostream>
#include <fstream> 
#include <string> 
using namespace std; 

int main() {
    string filename = "testo_es_1.txt"; 
    ifstream ifs(filename); 
    if ( ifs.is_open() ) {
        while( !ifs.eof() ) { 
            int i=0; 
            while (i<=2) {
                string città; 
                double temp1, temp2, temp3, temp4; 
                if (ifs >> città >> temp1 >>temp2 >> temp3 >>temp4) {
                    double media = (temp1+temp2+temp3+temp4)/4; 
                    cout << città << " " << media << "\n"; 
                }
                i++;
                
            } 
            
        }
        ifs.close();
    }
    return 0; 
}
