#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool debugOverride;
uniform vec4 debugColor;

void main()
{
    if (debugOverride) {
        FragColor = debugColor;
        return;
    }
    vec4 texColor = texture(texture1, TexCoord);
    if (texColor.a <= 0.01)
        discard;
    FragColor = texColor;
}