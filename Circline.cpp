//
//  Circline.cpp
//  PoincareFFGL
//
//  Created by Michael Dewberry on 7/16/13.
//
//

#include "Circline.h"

std::shared_ptr<Circline> Circline::create(float a, Complex b, float c) {
    if (Accuracy::lengthIsZero(a)) {
        return std::make_shared<Line>(b, c);
    }
    
    return std::make_shared<Circle>(a, b, c);
}

const Circle Circle::unit = *(dynamic_cast<Circle*>(Circle::create(Complex::zero, 1).get()));

std::shared_ptr<Circline> Circle::inverse() {
    if (Accuracy::lengthIsZero((center().modulusSquared() - radiusSquared()))) {
        return std::make_shared<Line>(b.negative().conjugate(), 1);
    }
    return std::make_shared<Circle>(center().modulusSquared() - radiusSquared(), b.conjugate(), 1);
}