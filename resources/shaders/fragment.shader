#version 410

uniform sampler2D tex; // texture
uniform vec4 matColor;

in vec2 fragTexCoord; // texture coordinate (u,v)

out vec4 finalColor; //

void main()
{
  vec4 texel = texture(tex, fragTexCoord);
  texel = clamp(texel + matColor, 0.0, 1.0);
  finalColor = texel;
  // finalColor = vec4(1.0,1.0,1.0,1.0);
}
