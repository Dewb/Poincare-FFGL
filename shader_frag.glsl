char mobiusFragmentShaderCode[] = R"(

varying vec2 vTextureCoord;
uniform sampler2D inputTexture;
uniform vec2 uTextureOffset;
uniform float uIsInverted;

void main(void) {
    //				gl_FragColor = vec4(1.0, 1.0, 1.0, 1);
    gl_FragColor = texture2D(inputTexture, vec2(vTextureCoord.s, vTextureCoord.t) - uTextureOffset);
    if (uIsInverted != 0.0) {
        gl_FragColor = vec4(
                            (1.0 - gl_FragColor.x) * uIsInverted + gl_FragColor.x * (1.0 - uIsInverted),
                            (1.0 - gl_FragColor.y) * uIsInverted + gl_FragColor.y * (1.0 - uIsInverted),
                            (1.0 - gl_FragColor.z) * uIsInverted + gl_FragColor.z * (1.0 - uIsInverted),
                            1) ;
    }
}

)";

char basicFragmentShaderCode[] = R"(

uniform sampler2D inputTexture;
varying vec2 vTextureCoord;
uniform vec2 uTextureOffset;

void main(void) {
    gl_FragColor = texture2D(inputTexture, vec2(vTextureCoord.s, vTextureCoord.t) - uTextureOffset);
    //	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

)";