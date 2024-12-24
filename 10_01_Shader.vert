//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul X - 10_01_Shader.vert |
// =====================================
// 
//  Shaderul de varfuri / Vertex shader - afecteaza geometria scenei; 
 
 #version 330 core


layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

//  Variabile de iesire;
out vec3 FragPos;
out vec3 Normal;
out vec3 inLightPos;
out vec3 inViewPos;
out vec4 ex_Color;

//  Variabile uniforme;
uniform int nrVertices;
uniform mat4 myMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 matrUmbra;
uniform int codCol;
uniform float xL, yL, zL;

vec3 viewPos = vec3(3, 2, 1);



void main(void)
  {
    if(codCol > 0){
        gl_Position = projection*view*myMatrix*vec4(in_Position, 1.0);
        FragPos = vec3(gl_Position);
    }
    else{
        gl_Position = projection*view*matrUmbra*myMatrix*vec4(in_Position, 1.0);
        FragPos = vec3(gl_Position);
    }
    Normal=vec3(projection*view*vec4(in_Normal,0.0));
    inLightPos= vec3(projection*view*myMatrix* vec4(xL, yL, zL, 1.0f));
    inViewPos=vec3(projection*view*vec4(viewPos, 1.0f));
	ex_Color=vec4(0.59, 0.29, 0.0, 1.0);
  } 
 