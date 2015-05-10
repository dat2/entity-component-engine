// libraries
#include <glm/gtc/type_ptr.hpp>

// engine
#include <components/render/light.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace render
{
  Light::Light(Json::Value value, EntityPtr entity)
    : Component(LIGHT), mAmbientCoefficient(value["ambient"].asFloat()), mAttenuationFactor(value["attenuation"].asFloat())
  {
    mColour = jsonToVector(value, "colour", "r", "g", "b");
  }

  const glm::vec3 Light::colour() const
  {
    return mColour;
  }
  const float Light::ambient() const
  {
    return mAmbientCoefficient;
  }
  const float Light::attenuation() const
  {
    return mAttenuationFactor;
  }

  void Light::print(std::ostream& where) const
  {
    printField(where, "colour", toString(mColour));
    printField(where, ", ambientCoefficient", mAmbientCoefficient);
    printField(where, ", attenuationFactor", mAttenuationFactor);
  }
}
