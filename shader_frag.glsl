char *fragmentShaderCode = R"(

uniform sampler2D inputTexture;
uniform sampler1D heatTexture;
uniform vec2 maxCoords;
uniform float heatAmount;
const vec4 grayScaleWeights = vec4(0.30, 0.59, 0.11, 0.0);

void main()
{
    vec2 texCoord = gl_TexCoord[1].st; //lookup input color
    vec4 c = texture2D(inputTexture, texCoord);
    float luminance = dot(c,grayScaleWeights); //calc luminance
    
    float heatCoord = luminance*heatAmount; //lookup heat based on luminance
    vec4 col_out = texture1D(heatTexture, heatCoord);
    col_out.a = 1.0;
    gl_FragColor = col_out;
}

)";