#ifndef TPOLYNOMIAL_H
#define TPOLYNOMIAL_H

/* including ROOT Framework class */
#include "TF1.h"

/* STL headers */
#include <string>
#include <vector>

class TPolynomial
{
    public:
    TPolynomial(int);
    ~TPolynomial();

    int deg{}; // polynomial expression degree
    std::string phrase{}; // string passed to a class TF1 object in order to compute the polynomial regression
    TF1* f{nullptr}; // TF1 (one-dimensional function) class from the ROOT Framework
    std::vector<double> parameters{}; // vector holding the parameters of a polynomial expression
    double chi2{}; // fit parameter
    int ndf{}; // number of degrees of freedom
};

#endif
