//
//  Circline.cpp
//  PoincareFFGL
//
//  Created by Michael Dewberry on 7/16/13.
//
//

#include "Circline.h"

Circline Circline::create(float a, Complex b, float c) {
    if (Accuracy::lengthIsZero(a)) {
        return Line(b, c);
    }
    
    return Circle(a, b, c);
}

const Circle Circle::unit = Circle::create(Complex::zero, 1);

Circline Circle::inverse() {
    if (Accuracy::lengthIsZero((center().modulusSquared() - radiusSquared()))) {
        return Line(b.negative().conjugate(), 1);
    }
    return Circle(center().modulusSquared() - radiusSquared(), b.conjugate(), 1);
}