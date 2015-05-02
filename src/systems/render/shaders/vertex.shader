#version 410

uniform mat4 camera; //mwc
uniform mat4 model;

layout(location=0) in vec3 vert; // (x,y,z)
layout(location=1) in vec2 vertTexCoord; // (u,v)
layout(location=2) in vec3 vertNormal; // (nx, ny, nz)

out vec3 fragVert; // (x,y,z)
out vec2 fragTexCoord; // (u,v)
out vec3 fragNormal; // (nx,ny,nz)

void main()
{
  // pass through vertices for diffuse lighting
  fragVert = vert;
  fragTexCoord = vertTexCoord;
  fragNormal = vertNormal;

  gl_Position = camera * model * vec4(vert, 1);
}
