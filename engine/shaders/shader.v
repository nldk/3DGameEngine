#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

// Flip flags: (1,0) = horizontal, (0,1) = vertical, (1,1) = both
uniform vec2 uFlip;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = mix(aTexCoord, vec2(1.0) - aTexCoord, uFlip);
}