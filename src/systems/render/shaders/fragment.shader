#version 410

uniform mat4 model;
uniform mat3 normalMatrix;

uniform sampler2D materialTexture;
uniform float materialShininess;
uniform vec3 materialSpecularColour;

uniform vec3 cameraPosition;

uniform float gamma;

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
   float ambientCoefficient;
   float attenuationFactor;
} light;

in vec3 fragVert; // (x,y,z)
in vec2 fragTexCoord; // (u,v)
in vec3 fragNormal;  // (nx,ny,nz)

out vec4 finalColor;

void main()
{
  vec4 surfaceColour = texture(materialTexture, fragTexCoord);

  // diffuse
  vec3 normal = normalize(normalMatrix * fragNormal);
  vec3 surfacePosition = vec3(model * vec4(fragVert, 1.0));
  vec3 surfaceToLight = normalize(light.position - surfacePosition);
  float diffuseCoefficient = max(0, dot(surfaceToLight, normal));

  vec3 diffuse = diffuseCoefficient * light.intensities * surfaceColour.rgb;

  // ambient
  vec3 ambient = light.ambientCoefficient * light.intensities * surfaceColour.rgb;

  // specular
  vec3 incidenceVector = -surfaceToLight;
  vec3 reflectionVector = reflect(incidenceVector, normal);
  vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition);

  // if diffuse <= 0, then we are in the back of the surface
  float specularCoefficient = 0.0;
  if(diffuseCoefficient > 0.0)
  {
    specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflectionVector)), materialShininess);
  }

  vec3 specular = specularCoefficient * light.intensities * materialSpecularColour;

  // attenuation
  float surfaceDistance = length(light.position - surfacePosition);
  float attenuation = 1.0 / (1.0 + light.attenuationFactor * pow(surfaceDistance, 2));

  // linear colour
  vec3 linearColour = ambient + attenuation * (diffuse + specular);

  // gamma correction
  vec3 gamma = vec3(1.0 / gamma);
  vec3 finalColour = pow(linearColour, gamma);

  finalColor = clamp(vec4(linearColour, surfaceColour.a), 0.0, 1.0);
}
