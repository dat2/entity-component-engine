#version 410

uniform mat4 model;
uniform sampler2D materialTexture;

uniform mat3 normalMatrix;
uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
} light;

in vec3 fragVert; // (x,y,z)
in vec2 fragTexCoord; // (u,v)
in vec3 fragNormal;  // (nx,ny,nz)

out vec4 finalColor; //

void main()
{
  vec4 surfaceColour = texture(materialTexture, fragTexCoord);

  // diffuse component of the phong reflection model
  vec3 normal = normalize(normalMatrix * fragNormal);
  vec3 fragPosition = vec3(model * vec4(fragVert, 1.0));
  vec3 surfaceToLight = normalize(light.position - fragPosition);
  float diffuseCoefficient = max(0,dot(surfaceToLight, normal));

  vec3 diffuse = diffuseCoefficient * light.intensities * surfaceColour.rgb;

  // ambient
  // vec3 ambient = light.ambientCoefficient * light.intensities * surfaceColour.rgb;

  // specular


  vec4 phongColour = vec4(diffuse /*+ ambient*/, surfaceColour.a);
  finalColor = clamp(phongColour, 0.0, 1.0);
  // finalColor = vec4(1.0,1.0,1.0,1.0);
}
