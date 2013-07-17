//
//  Circline.h
//  PoincareFFGL
//
//  Created by Michael Dewberry on 7/16/13.
//
//

#pragma once

#include "Complex.h"

class Circle;
class Line;

class Circline {
public:
    float a;
    Complex b;
    float c;
    
    Circline(float aa, Complex bb, float cc)
    : a(aa), b(bb), c(cc)
    {
        //if(isNaN(a) || isNaN(b.data[0] || isNaN(b.data[1]) || isNaN(c)))	{
        //    var d = 0; // tbd throw
        //}
    }

    static std::shared_ptr<Circline> create(float a, Complex b, float c);


    std::shared_ptr<Circline> transform(const Mobius& mobius) {
        Mobius inverse = mobius.inverse();
        //			Mobius hermitian = inverse.Transpose *
        //			new Mobius(new Complex(Circline.a, 0), Circline.b.Conjugate, Circline.b, new Complex(Circline.c, 0)) *
        //			inverse.Conjugate;
        Mobius m1 = inverse.transpose();
        Mobius m2(Complex(a, 0), b.conjugate(), b, Complex(c, 0));
        Mobius m3 = inverse.conjugate();
	
        Mobius hermitian = Mobius::multiply(Mobius::multiply(m1, m2), m3);
        return create(hermitian.a.real, hermitian.c, hermitian.d.real);
    }

    bool equals(const Circline& other) const {
        return Accuracy::lengthEquals(a, other.a) &&
               Complex::equals(b, other.b) &&
               Accuracy::lengthEquals(c, other.c);
    }

    bool isPointOnLeft(const Complex& point) {
        float sum =
            a * point.modulusSquared() +
            Complex::add(Complex::multiply(b.conjugate(), point),
                         Complex::multiply(b, point.conjugate())).real +
            c + Accuracy::linearTolerance;
        return sum > 0;
    }

    bool containsPoint(const Complex& point) {
        return Accuracy::lengthIsZero(a * point.modulusSquared() +
                                      Complex::add(Complex::multiply(b.conjugate(), point),
                                                   Complex::multiply(b, point.conjugate())).real
                                      + c);
    }

    bool arePointsOnSameSide(const Complex& p1, const Complex& p2) {
        if (Complex::equals(p1, p2))
            return true;
    
        return isPointOnLeft(p1) ^ isPointOnLeft(p2);
    };

    std::shared_ptr<Circline> conjugate() {
        return create(a, b.conjugate(), c);
    }
    
    virtual std::shared_ptr<Circline> inverse() = 0;
    virtual Mobius asMobius() = 0;
    
};

class Circle : public Circline
{
public:
    Circle(float aa, const Complex& bb, float cc)
    : Circline(1, bb.scale(1/aa), cc/aa)
    {
    }

    static std::shared_ptr<Circline> create(const Complex& center, float radius) {
        return std::make_shared<Circle>(1, center.negative(), center.modulusSquared() - radius * radius);
    }

    static const Circle unit;

    Complex center() {
        return b.negative().scale(1/a);
    }

    float radiusSquared() {
        return center().modulusSquared() - c / a;
    }

    float radius() {
        return sqrt(radiusSquared());
    }

    virtual std::shared_ptr<Circline> inverse();

    virtual Mobius asMobius() {
        return Mobius(center(),
                      Complex(radiusSquared() - center().modulusSquared(), 0),
                      Complex::one,
                      b.conjugate());
    }

    std::shared_ptr<Circline> scale(const Complex& c) {
        return scale(c.modulus());
    }
    
    std::shared_ptr<Circline> scale(float s) {
        return Circle::create(center().scale(s), radius() * s);
    }
};


class Line : public Circline {
public:
    Line(const Complex& bb, float cc)
    : Circline(0, bb, cc)
    {
    }

    static std::shared_ptr<Line> createTwoPoint(const Complex& p1, const Complex& p2) {
        float dx = p2.real - p1.real;
        float dy = p2.imag - p1.imag;
    
        return Line::createFromEquation(-dy, dx, dx * p1.imag - dy * p1.real);
    }

    // Creates a linear Circle a * x + b * y + c = 0 from reals a, b, and c.
    static std::shared_ptr<Line> createFromEquation(float a, float b, float c) {
        return std::make_shared<Line>(Complex(a / 2.0, b / 2.0), c);
    }

    static std::shared_ptr<Line> createPointAngle(const Complex& point, float angle) {
        return Line::createTwoPoint(point, Complex::subtract(point, Complex::createPolar(1, angle)));
    }

    virtual std::shared_ptr<Circline> inverse() {
        if (Accuracy::lengthIsZero(c)) {
            return std::make_shared<Line>(b.conjugate(), 0);
        }
    
        return Circle::create(b.conjugate().scale(1 / c), b.modulus() / c);
    }

    virtual Mobius asMobius() {
        return Mobius(b, Complex::one, Complex::zero, b.negative().conjugate());
    }

};