#version 410

uniform mat4 camera; //mwc
uniform mat4 model;

layout(location=0) in vec3 vert; // (x,y,z)
layout(location=1) in vec2 vertTexCoord; // (u,v)

out vec2 fragTexCoord; // (u,v)

void main()
{
  fragTexCoord = vertTexCoord; // pass uv through unchanged
  gl_Position = camera * model * vec4(vert, 1);
}
