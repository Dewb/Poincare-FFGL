//
//  Complex.cpp
//  PoincareFFGL
//
//  Created by Michael Dewberry on 7/15/13.
//
//

#include "Complex.h"

#include <sstream>

const Complex Complex::zero = Complex(0.0, 0.0);
const Complex Complex::one = Complex(1.0, 0.0);
const Complex Complex::i = Complex(0.0, 1.0);

std::string Complex::toString() {
    std::ostringstream ss;
    ss << "{" << real << ", " << imag << "}";
    return ss.str();
};

Complex Complex::transform(const Mobius& mobius) const {
    return divide(add(multiply(mobius.a, *this), mobius.b), add(multiply(mobius.c, *this), mobius.d));
};

Mobius const Mobius::identity(Complex::one, Complex::zero, Complex::zero, Complex::one);