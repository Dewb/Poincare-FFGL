char vertexShaderCode[] = R"(

    attribute vec3 aVertexPosition;
    attribute vec2 aTextureCoord;

    uniform vec2 uMobiusA;
    uniform vec2 uMobiusB;
    uniform vec2 uMobiusC;
    uniform vec2 uMobiusD;

    uniform float uInterp;

    vec2 pos;
    varying vec2 vTextureCoord;

    vec2 one = vec2(1.0, 0.0);
    vec2 i = vec2(0.0, 1.0);
    vec2 zero = vec2(0.0, 0.0);
    float pi = 4.0 * atan(1.0); 

    vec2 fromPolar(in float modulus, in float argument) {
        return vec2(modulus * cos(argument), modulus * sin(argument));
    }

    vec2 cMultiply(in vec2 a, in vec2 b) {
        return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
    }

    vec2 cDivide(in vec2 a, in vec2 b) {
        float automorphy = b.x * b.x + b.y * b.y;
        return vec2((a.x * b.x + a.y * b.y) / automorphy, (a.y * b.x - a.x * b.y) / automorphy);
    }

    vec2 cExp(in vec2 z) {
        float modulus = length(z);
        float argument = atan(z.y, z.x);
        return fromPolar(modulus, argument);
    }

    vec2 cLog(in vec2 z) {
        return vec2(log(length(z)), atan(z.y, z.x));
    }

    // For perfect band, use: pos = (4.0 / pi) * cAtanh(pos);
    vec2 cAtanh(in vec2 z) {
        return 0.5 * (cLog(one + z) - cLog(one - z));
    }

    vec2 cSin(in vec2 z) {
        return 0.5 * cMultiply(i, exp(cMultiply(i, z)) - exp(cMultiply(-1.0 * i, z)));
    }

    // Band from http://bulatov.org/math/1107/index_talk_web.html
    vec2 cStripMap(in vec2 z, in float t) {
        if (t == 0.0) 
            return z; 

        float c = 1.0 - log(t);
        vec2 cVec2 = vec2(c, 0);
        float scale = 1.0 / (cLog(cVec2 + i) - cLog(cVec2 - i)).y;

        const int cutCount = 4;
        vec2 cuts[cutCount];
        cuts[0] = i;
    //		cuts[1] = -i;
    //		cuts[2] = one;
    //		cuts[3] = -one;

        float sq2 = sqrt(2.0)/2.0;
        cuts[0] = fromPolar(1.0, pi/4.0);
        cuts[1] = fromPolar(1.0, 3.0 * pi/4.0);
        cuts[2] = fromPolar(1.0, -pi/4.0);
        cuts[3] = fromPolar(1.0, -3.0 * pi/4.0);

        vec2 result = zero;
        for (int index = 0; index < cutCount; index++) {
            float sign = index == 2 || index == 1 ? 1.0 : -1.0;
            //float sign = -1.0;
            result += sign * cMultiply(cuts[index], cLog(cVec2 + cMultiply(cuts[index], z)));
        }

    //		vec2 a = i;
    //		vec2 b = -i;
    //		return 0.25 * (
    //			cMultiply(a, cLog(cVec2 - cMultiply(a, z))) 
    //			+ cMultiply(b, cLog(cVec2 - cMultiply(b, z))) 
    //			- cLog(cVec2 - z)
    //		);

        return 0.25 * scale * result;
    // original	return scale * (cLog(cVec2 + z) - cLog(cVec2 - z));
    }

    vec2 cHalfPlaneMap(in vec2 z, in float t) {
        z = cDivide(
            cMultiply(i, z) + one,
            cMultiply(one, z) + i
        );
        return z + i;
    }

    void main(void) {
        pos = aVertexPosition.xy;

        pos = cDivide(
            cMultiply(uMobiusA, pos) + uMobiusB,
            cMultiply(uMobiusC, pos) + uMobiusD
        );

        pos = cStripMap(pos, uInterp);
        //pos = cHalfPlaneMap(pos, 0.5);

        gl_Position =  vec4(pos, 0.0, 1.0);
        vTextureCoord = aTextureCoord;
    }

)";