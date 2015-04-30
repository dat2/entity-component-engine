#version 410

uniform sampler2D tex; // texture
uniform vec4 matColor;

in vec2 TexCoord; // texture coordinate (u,v)

out vec4 finalColor; //

void main()
{
  vec4 texel = texture(tex, TexCoord);
  texel = clamp(texel + matColor, 0.0, 1.0);
  finalColor = texel;
}
