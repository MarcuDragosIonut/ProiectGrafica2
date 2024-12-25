#version 330 core

in vec3 st_Color;
in vec3 st_FragPos;

// TODO \/
in vec3 Normal;
in vec3 inLightPos;
in vec3 inViewPos;

out vec4 out_Color;

uniform int codCol;

vec3 lightColor = vec3(0.7, 0.9, 0.6);

void main(void)
{
    if (codCol > 0) {
        //  Ambient;
        float ambientStrength = 0.4f;
        vec3 ambient_light = ambientStrength * lightColor;
        vec3 ambient_term = ambient_light * st_Color;

        //  Diffuse;
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(inLightPos - st_FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse_light = lightColor;
        vec3 diffuse_term = diff * diffuse_light * st_Color;

        //  Specular;
        float specularStrength = 0.8f;
        float shininess = 50.0f;
        vec3 viewDir = normalize(inViewPos - st_FragPos);
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular_light = specularStrength * lightColor;
        vec3 specular_term = spec * specular_light * st_Color;

        //  Culoarea finala;
        vec3 emission = vec3(0.0, 0.0, 0.0);
        vec3 result = emission + (ambient_term + diffuse_term + specular_term);
        out_Color = vec4(result, 1.0f);
    }
    if (codCol == 0) {
        out_Color = vec4(vec3(0.0, 0.0, 0.0), 1.0);
    }
}
// TODO /\
