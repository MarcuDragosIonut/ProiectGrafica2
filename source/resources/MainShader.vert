#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
layout (location = 2) in vec3 in_Normal;

out vec3 st_Color;
out vec3 st_FragPos;

uniform bool isShadow;

// TODO v
out vec3 Normal;
out vec3 inLightPos;
out vec3 inViewPos;

uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 matrUmbra;
uniform float xL, yL, zL;

vec3 viewPos = vec3(3, 2, 1);
// TODO ^

void main(void)
{
    if (isShadow)
    {
        // TODO v
        gl_Position = projection * view * matrUmbra * modelMatrix * vec4(in_Position, 1.0);
        // TODO ^
        return;
    }

    // TODO v
    gl_Position = projection * view * modelMatrix * vec4(in_Position, 1.0);
    Normal = vec3(projection * view * vec4(in_Normal, 0.0));
    inLightPos = vec3(projection * view * modelMatrix * vec4(xL, yL, zL, 1.0f)); // sursa punctuala
    inViewPos = vec3(projection * view * vec4(viewPos, 1.0f));
    // TODO ^

    st_Color = in_Color;
    st_FragPos = vec3(gl_Position);
}
