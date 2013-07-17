//
//  Face.cpp
//  PoincareFFGL
//
//  Created by Michael Dewberry on 7/17/13.
//
//

#include "Face.h"

Region::Region(float pp, float qq)
{
    p = pp;
    q = qq;
    float sinP2 = pow(sin(PI / p), 2);
    float cosQ2 = pow(cos(PI / q), 2);
    r = sqrt(sinP2 / (cosQ2 - sinP2));
    d = sqrt(cosQ2 / (cosQ2 - sinP2));
    phi = PI * (0.5 - (1.0 / p + 1.0 / q));
    
    l1 = Line::createTwoPoint(Complex::zero, Complex::one);
    l2 = Line::createPointAngle(Complex::zero, PI / p);
    c = std::dynamic_pointer_cast<Circle>(Circle::create(Complex(d, 0), r));
    Complex center = c->center();
    
    Complex polar = Complex::createPolar(r, PI - phi);
    p0 = Complex::zero;
    p1 = Complex::add(Complex(d, 0), polar);
    p2 = Complex(d - r, 0);
    
    /*         08
     *       09  07
     *     10  14  06
     *   11  12  13  05
     * 00  01  02  03  04
     * */
    
    points.resize(15);
    int count = 4;
    for (int i = 0; i < count; i++) {
        float t = i / count;
        points[i] = p2.scale(t);
        points[i + count] = Complex::add(Complex(d, 0), Complex::createPolar(r, PI - phi * t));
        points[i + 2 * count] = p1.scale(1 - t);
    }
    
    points[12] = Complex::add(p0, Complex::add(p1, p2).scale(0.5)).scale(0.5);
    points[13] = Complex::add(p2, Complex::add(p0, p1).scale(0.5)).scale(0.5);
    points[14] = Complex::add(p1, Complex::add(p2, p0).scale(0.5)).scale(0.5);
}
