#include <iostream>
#include <Eigen/Dense>
#include <Eigen/SVD> 

//Funzione per calcolare il numero di condizionamento di una matrice A 
double condA(const Eigen::MatrixXd& A)
{
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A); //Decomposizione ai valori singolari della matrice A 
  Eigen::VectorXd singularValuesA = svd.singularValues(); //Vettore con i valori singolari 
  return singularValuesA.maxCoeff() / singularValuesA.minCoeff(); //Numero di condizionamento (Valore singolare massimo fratto minimo)
}

int main(int argc, char **argv) { 
  const double tol = 1.0e-15;
  unsigned int n = 3;

  Eigen::MatrixXd B = Eigen::MatrixXd::Random(n, n); //Genero una matrice casuale B 
  if(abs(B.determinant())<tol) { 
    return -1; 
  }

  Eigen::MatrixXd A = B.transpose() * B; //Rendo A simmetrica e definita positiva
  Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n); //Soluzione esatta (vettore di uni)

  Eigen::VectorXd b = A * x_ex; //Calcolo b termine noto

  Eigen::VectorXd x = Eigen::VectorXd::Zero(n); //x0 vettore iniziale arbitrario
  Eigen::VectorXd res = b - A * x; //residuo iniziale r0
  Eigen::VectorXd p = res; //p0 = r0 prima direzione è r0
  double res_norm_0 = res.norm(); //Norma iniziale r0

  double res_norm = res_norm_0; //Per tenere conto del residuo 

  const unsigned int it_max = 10000; //Numero massimo iterazioni 
  unsigned int it = 0;
  const double res_tol = 1.0e-12; //Tolleranza per arrestare il ciclo 

  std::cout.precision(2);
  std::cout<< std::scientific<< "Matrix Cond: "<< condA(A)<< std::endl;

  //Ciclo, mi fermo se raggiungo la tolleranza o se supero il limite di iterazioni it_max 
  while(it < it_max && (res_norm/res_norm_0) > res_tol) { 
    Eigen::VectorXd Ap = A*p; 
    double p_A_p = p.dot(Ap); //Denominatore, .dot p per prodotto scalare
    double alpha_k = p.dot(res)/p_A_p; //Calcolo di alpha_k 

    x = x+(alpha_k*p); //Soluzione x 

    res = b-A*x; //Residuo 
    res_norm = res.norm(); 

    double beta_k = p.dot(A*res)/p_A_p; //Calcolo coefficiente beta_k 

    p = res-beta_k*p; //Aggiorno la direzione p 

    it++;
  }

  //Errore finale
  const auto err_rel = (x_ex.norm() == 0.0) ? (x - x_ex).norm() :
                                              (x - x_ex).norm() / x_ex.norm();
  
  std::cout.precision(4);
  std::cout<< std::scientific << "\n" << "it"<< "/"<< "it_max"<< std::endl;
  std::cout<< std::scientific << it<< "/"<< it_max<< std::endl;
  std::cout<< std::scientific << "\n" << "res"<< "/"<< "res_tol"<< std::endl; //Residuo e tolleranza del residuo (residuo minimo richiesto)
  std::cout<< std::scientific << res.norm() / res_norm_0<< "/"<< res_tol<< std::endl;
  std::cout<< std::scientific << "\n" << "err_rel"<< std::endl;
  std::cout<< std::scientific << err_rel<< std::endl;
  std::cout<< std::scientific << "\n" "soluzione"<< std::endl;
  std::cout<< std::scientific << x << std::endl;
} 