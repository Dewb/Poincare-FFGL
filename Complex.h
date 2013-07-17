//
//  Complex.h
//  PoincareFFGL
//
//  Created by Michael Dewberry on 7/15/13.
//
//

#pragma once

#include <cmath>
#include <vector>
#include <string>

#define PI 3.141592f


class Accuracy {
public:
    static constexpr float linearTolerance = 1E-6;
    static constexpr float angularTolerance = 1E-2;
    static constexpr float linearToleranceSquared = linearTolerance * linearTolerance;
    static constexpr float maxLength = 100;

    static float lengthEquals(float a, float b) {
        return fabs(a - b) < linearTolerance;
    };

    static float lengthIsZero(float a) {
        return fabs(a) < linearTolerance;
    };

    static float angleEquals(float a, float b) {
        return fabs(a - b) < angularTolerance;
    };

    static float angleIsZero(float a) {
        return fabs(a) < angularTolerance;
    };

};

class Mobius;

class Complex
{
public:
    Complex() : real(0), imag(0) {}
    Complex(float r, float i) : real(r), imag(i) {}

    static const Complex zero;
    static const Complex one;
    static const Complex i;
    
    float real;
    float imag;
    
    std::string toString();

    static Complex add(const Complex& a, const Complex& b) {
        return Complex(a.real + b.real, a.imag + b.imag);
    };
    
    static Complex subtract(const Complex& a, const Complex& b) {
        return Complex(a.real - b.real, a.imag - b.imag);
    };
    
    static Complex multiply(const Complex& a, const Complex& b) {
        return Complex(a.real * b.real - a.imag * b.imag, a.real * b.imag + a.imag * b.real);
    };

    static Complex divide(const Complex& a, const Complex& b) {
        float automorphy = b.real * b.real + b.imag * b.imag;
        return Complex((a.real * b.real + a.imag * b.imag) / automorphy,
                       (a.imag * b.real - a.real * b.imag) / automorphy);
    };
                        
    static Complex createPolar(float r, float theta) {
        return Complex(r * cos(theta), r * sin(theta));
    };
    
    static std::vector<Complex> transformArray(std::vector<Complex>& original, Mobius& mobius) {
        std::vector<Complex> transformed;
        transformed.resize(original.size());
        for (int i = 0; i < original.size(); i++) {
            transformed[i] = original[i].transform(mobius);
        }
        
        return transformed;
    };

    static std::vector<Complex> conjugateArray(std::vector<Complex>& original) {
        std::vector<Complex> transformed;
        transformed.resize(original.size());
        for (int i = 0; i < original.size(); i++) {
            transformed[i] = original[i].conjugate();
        }
        
        return transformed;
    };
    
    Complex scale(float s) const {
        return Complex(real * s, imag * s);
    };

    Complex scale(Complex& c) const {
        return scale(c.modulus());
    };

    static bool equals(const Complex& a, const Complex& b) {
        return subtract(a, b).modulusSquared() < Accuracy::linearTolerance * Accuracy::linearTolerance;
    };
    
    float modulus() const {
        return sqrt(real * real + imag * imag);
    };
    
    float modulusSquared() const {
        return real * real + imag * imag;
    };
    
    float argument() const {
        return atan2(imag, real);
    };
    
    Complex negative() const {
        return Complex(-real, -imag);
    };
    
    Complex transform(const Mobius& mobius) const;
    
    Complex conjugate() const {
        return Complex(real, -imag);
    };
};

class Mobius {
public:
    Complex a;
    Complex b;
    Complex c;
    Complex d;
    
    Mobius(const Complex& aa, const Complex& bb, const Complex& cc, const Complex& dd)
    : a(aa), b(bb), c(cc), d(dd)
    {
        /*
         Complex det = Complex::subtract(
         Complex::multiply(a, d),
         Complex::multiply(b, c)
         );
         
         if (!Accuracy::lengthIsZero(det)) {
             a = Complex::divide(a, det);
             b = Complex::divide(b, det);
             c = Complex::divide(c, det);
             d = Complex::divide(d, det);
         }
         */
    }

    static const Mobius identity;
    
    static Mobius multiply(const Mobius& m1, const Mobius& m2) {
        return Mobius(
            Complex::add(Complex::multiply(m1.a, m2.a), Complex::multiply(m1.b, m2.c)),
            Complex::add(Complex::multiply(m1.a, m2.b), Complex::multiply(m1.b, m2.d)),
            Complex::add(Complex::multiply(m1.c, m2.a), Complex::multiply(m1.d, m2.c)),
            Complex::add(Complex::multiply(m1.c, m2.b), Complex::multiply(m1.d, m2.d))
        );
    }

    static Mobius add(const Mobius& m1, const Mobius& m2) {
        return Mobius(
            Complex::add(m1.a, m2.a),
            Complex::add(m1.b, m2.b),
            Complex::add(m1.c, m2.c),
            Complex::add(m1.d, m2.d)
        );
    }

    Mobius scale(float s) const {
        return Mobius(
            a.scale(s),
            b.scale(s),
            c.scale(s),
            d.scale(s)
        );
    }

    static Mobius createDiscAutomorphism(Complex& a, float phi) {
        return multiply(createRotation(phi),
                        Mobius(Complex::one, a.negative(), a.conjugate(), Complex::one.negative()));
    }

    static Mobius createDiscTranslation(Complex& a, Complex& b) {
        return multiply(createDiscAutomorphism(b, 0),
                        createDiscAutomorphism(a, 0).inverse());
    }

    static Mobius createTranslation(Complex translation) {
        return Mobius(Complex::one, translation, Complex::zero, Complex::one);
    }

    static Mobius createRotation(float phi) {
        return Mobius(Complex::createPolar(1, phi), Complex::zero, Complex::zero, Complex::one);
    }

    Mobius inverse() const {
        return Mobius(d, b.negative(), c.negative(), a);
    }

    Mobius conjugate() const {
        return Mobius(a.conjugate(), b.conjugate(), c.conjugate(), d.conjugate());
    }

    Mobius transpose() const {
        return Mobius(a, c, b, d);
    }

    Mobius conjugateTranspose() const {
        return Mobius(a.conjugate(), c.conjugate(), b.conjugate(), d.conjugate());
    }
};


class ComplexCollection
{
public:
	static constexpr int sectorCount = 64;
	static constexpr int annulusCount = 128;
    
    int maximum = 0;
    
    std::vector<Complex> sectors[sectorCount][annulusCount];

    void add(Complex& c) {
        int sector = getSector(c);
        float modulusSquared = c.modulusSquared();
        int annulus = floor(modulusSquared * annulusCount);
	
        sectors[sector][annulus].push_back(c);
        maximum = fmax(maximum, modulusSquared);
    }

    bool contains(Complex& c) {
        int sector = getSector(c);
        int annulus = floor(c.modulusSquared() * annulusCount);
	
        for (int i = 0; i < sectors[sector][annulus].size(); i++) {
            if (Complex::equals(sectors[sector][annulus][i], c)) {
                return true;
            }
        }
	
        return false;
    }

    void clear() {
        for (int i = 0; i < sectorCount; i++) {
            for (int j = 0; j < annulusCount; j++) {
                sectors[i][j].clear();
            }
        }
    }

    int getSector(Complex& c) {
        return (int)floor(((c.argument() + PI) / (PI * 2.0 / (sectorCount * 1.0)))) % sectorCount;
    }
};



