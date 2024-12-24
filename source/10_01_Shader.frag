//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul X - 10_01_Shader.frag |
// =====================================
// 
//  Shaderul de fragment / Fragment shader - afecteaza culoarea pixelilor;
//


#version 330 core

in vec3 FragPos;  
in vec3 Normal; 
in vec3 inLightPos;
in vec3 inViewPos;
in vec4 ex_Color;

out vec4 out_Color;

uniform int codCol; 
uniform float fCeata;

vec3 lightColor = vec3 (0.7, 0.9, 0.6);
vec3 objectColor = ex_Color.xyz;

vec3 culoareCeata = vec3(0.4, 0.4, 0.4);

void main(void)
  {
    if(codCol > 0){

        if(codCol == 2){
            objectColor = vec3(0.8, 0.6, 0.1);
        }
        //  Ambient;
        float ambientStrength = 0.4f;
        vec3 ambient_light = ambientStrength * lightColor;
        vec3 ambient_term= ambient_light * objectColor;
  	
        //  Diffuse; 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(inLightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0); 
        vec3 diffuse_light = lightColor;
        vec3 diffuse_term = diff * diffuse_light * objectColor;
    
        //  Specular;
        float specularStrength = 0.8f;
        float shininess = 50.0f;
        vec3 viewDir = normalize(inViewPos - FragPos);
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); 
        vec3 specular_light = specularStrength  * lightColor;
        vec3 specular_term = spec * specular_light * objectColor;
       
        //  Culoarea finala; 
        vec3 emission=vec3(0.0, 0.0, 0.0);
        vec3 result = emission + (ambient_term + diffuse_term + specular_term);
	    out_Color = vec4(result, 1.0f) * fCeata + vec4(culoareCeata, 1.0f) * (1 - fCeata);
    }
    if (codCol == 0){
        out_Color = vec4(vec3(0.0,0.0,0.0), 1.0) * fCeata  + vec4(culoareCeata, 1.0f) * (1 - fCeata);
    }
  }