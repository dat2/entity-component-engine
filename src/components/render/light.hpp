#ifndef _LIGHT_HPP
#define _LIGHT_HPP

// libraries
#include <iostream>
#include <GL/GLEW.h>
#include <glm/vec3.hpp>
#include <json/json.h>

//engine
#include <components/component.hpp>
#include <entities/entity.hpp>

using namespace components;
using namespace entities;

namespace render
{
  class Light : public Component
  {
  public:
    Light(Json::Value value, EntityPtr entity);

    const glm::vec3 colour() const;
    const float ambient() const;
    const float attenuation() const;

    void print(std::ostream& where) const;

  private:
    glm::vec3 mColour;
    float mAmbientCoefficient;
    float mAttenuationFactor;
  };

}

#endif
