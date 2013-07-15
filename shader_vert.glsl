char *vertexShaderCode = R"(

void main()
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_TexCoord[1] = gl_MultiTexCoord1;
  gl_FrontColor = gl_Color;
}

)";