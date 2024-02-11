#include "tpolynomial.h"

#include <iostream>
// #include <cstring>

TPolynomial::TPolynomial(int d) : deg(d)
{
    phrase = "pol " + std::to_string(d);
    f = new TF1("fit", phrase.c_str());
    // std::cout << "TPolynomial Constructor" << std::endl;
}

TPolynomial::~TPolynomial()
{
    delete f;
    // std::cout << " ~TPolynomial Destructor" << std::endl;
}
