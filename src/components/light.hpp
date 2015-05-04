#ifndef _LIGHT_HPP
#define _LIGHT_HPP

// libraries
#include <iostream>
#include <GL/GLEW.h>
#include <glm/vec3.hpp>
#include <json/json.h>

//engine
#include <components/component.hpp>
#include <systems/render/program.hpp>

using namespace systems;

namespace components
{
  class Light : public Component
  {
  public:
    Light(Json::Value value);

    void setProgramVariables(Program &program, const GLchar* position, const GLchar* colour, const GLchar* ambient, const GLchar* attenuation);

    void print(std::ostream& where) const;

  private:
    glm::vec3 mPosition;
    glm::vec3 mColour;
    float mAmbientCoefficient;
    float mAttenuationFactor;
  };

}

#endif
