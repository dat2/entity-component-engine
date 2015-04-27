#version 410

uniform sampler2D wood; // texture

in vec2 fragTexCoord; // texture coordinate (u,v)

out vec4 finalColor; //

void main()
{
  // finalColor = vec4(1.0, 1.0, 1.0, 1.0);
  finalColor = texture(wood, fragTexCoord);
}
