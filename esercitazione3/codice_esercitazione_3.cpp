#include <iostream>
#include <concepts> 
#include <numeric>  

template<typename I> requires std::integral<I>
class rational {
    I num_;
    I den_;

    //Semplifico la frazione 
    void semplificazione() {
        if (den_ == 0) { 
            return; // Se è Inf o NaN, non semplifico
        } 
        
        I divisore = std::gcd(num_, den_); //Calcolo il massimo comune divisore tra numeratore e denominatore
        num_ /= divisore;
        den_ /= divisore;

        if (den_ < 0) {
            num_ = -num_; 
            den_ = -den_; 
        }
    }

public:
    // 1. Costruttore di default
    rational() : num_(0), den_(1) {}

    // 2. Costruttore user-defined 
    rational(I n, I d) : num_(n), den_(d) {
        if (den_ != 0) {
            semplificazione(); //Semlifico se num è diverso da zero
        }
    }

    I num() const { return num_; }
    I den() const { return den_; }

    // Operazione +=
    rational& operator+=(const rational& other) {
        if (den_ == 0 || other.den_ == 0) { 
            if (num_ == 0 || other.num_ == 0) { // || significa or
                num_ = 0; den_ = 0; //Se almeno uno dei due addendi è zero su zero, pongo num e den uguali a zero 
            } 
            else { 
                den_ = 0; 
            } 
        } 
        else {
            num_ = num_ * other.den_ + other.num_ * den_; 
            den_ *= other.den_;
            semplificazione(); //Semplifico numeratore e denominatore ottenuti dalla somma
        }
        return *this; 
    }

    //Operazione -=
    rational& operator-=(const rational& other) { 
        if (den_ == 0 || other.den_ == 0) {
            if (num_ == 0 || other.num_ == 0) { 
                num_ = 0; den_ = 0; 
            }
            else { 
                den_ = 0; 
            }
        } 
        else {
            num_ = num_ * other.den_ - other.num_ * den_; //Uguale a +=, ma qui meno
            den_ *= other.den_;
            semplificazione();
        }
        return *this;
    }

    //Operazione *=
    rational& operator*=(const rational& other) {
        num_ *= other.num_;
        den_ *= other.den_;
        semplificazione();
        return *this;
    }

    //Operazione /=
    rational& operator/=(const rational& other) {
        num_ *= other.den_;
        den_ *= other.num_;
        semplificazione();
        return *this;
    }

    //Operazione +
    rational operator+(const rational& other) const { 
        rational ret = *this; 
        ret += other; 
        return ret; 
    }

    //Operazione -
    rational operator-(const rational& other) const { 
        rational ret = *this; 
        ret -= other; 
        return ret; 
    }

    //Opererazione *
    rational operator*(const rational& other) const { 
        rational ret = *this; 
        ret *= other; 
        return ret; 
    }

    //Operazione /
    rational operator/(const rational& other) const { 
        rational ret = *this; 
        ret /= other; 
        return ret; 
    }

    
};

template<typename I>
std::ostream& operator<<(std::ostream& os, const rational<I>& r) {
    if (r.den() == 0) { 
        if (r.num() == 0) os << "NaN";
        else os << "Inf";
    } else {
        os << r.num() << "/" << r.den(); 
    }
    return os;
}

int main() {
    rational<int> a(6, 12);
    rational<int> b(1, 4); 

    std::cout << "\nOperazioni +, -, *, / : " << std::endl;
    std::cout << a << " + " << b << " = " << a + b << "\n";
    std::cout << a << " - " << b << " = " << a - b << "\n";
    std::cout << a << " * " << b << " = " << a * b << "\n";
    std::cout << a << " / " << b << " = " << a / b << "\n";

    std::cout << "\n" << "Operazioni +=, -=, *=, /= : " << "\n";
    rational<int> h = a;
    h += b; std::cout << a << " += " << b << " -> " << h << "\n";
    rational<int> j = a; 
    j -= b; std::cout << a << " -= " << b << " -> " << j << "\n";
    rational<int> m = a; 
    m *= b; std::cout << a << " *= " << b << " -> " << m << "\n";
    rational<int> v = a; 
    v /= b; std::cout << a << " /= " << b << " -> " << v << "\n";

    std::cout << "\n" << "Inf e NaN" << "\n";
    rational<int> l(1, 0);
    rational<int> f(0, 0);
    std::cout << a << " + Inf = " << a + l << "\n";
    std::cout << a << " + NaN = " << a + f << "\n";
    std::cout << "Inf * 2 = " << l * rational<int>(2, 1) << "\n"; 
    std::cout << "Inf * NaN = " << l * f << "\n"; 
    std::cout << "Inf / 8 = " << l / rational<int>(8, 1) << "\n"; 

    return 0;
}