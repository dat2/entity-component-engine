#version 410

uniform mat4 camera; //mwc
uniform mat4 model;

in vec3 vert; // (x,y,z)

in vec2 vertTexCoord; // (u,v)
out vec2 TexCoord; // (u,v)

void main()
{
  TexCoord = vertTexCoord; // pass uv through unchanged
  gl_Position = camera * model * vec4(vert, 1);
}
