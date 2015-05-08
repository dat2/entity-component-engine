// libraries
#include <glm/gtc/type_ptr.hpp>

// engine
#include <components/render/light.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace render
{
  Light::Light(Json::Value value)
    : Component(LIGHT), mAmbientCoefficient(value["ambient"].asFloat()), mAttenuationFactor(value["attenuation"].asFloat())
  {
    auto col = value["colour"];
    mColour = glm::vec3(col["r"].asFloat(), col["g"].asFloat(), col["b"].asFloat());

    auto pos = value["position"];
    mPosition = glm::vec3(pos["x"].asFloat(), pos["y"].asFloat(), pos["z"].asFloat());
  }

  void Light::setProgramVariables(Program &program, const GLchar* position, const GLchar* colour, const GLchar* ambient, const GLchar* attenuation)
  {
    GLint lightPositionIndex = program.uniform(position);
    glUniform3fv(lightPositionIndex, 1, glm::value_ptr(mPosition));

    GLint intensityIndex = program.uniform(colour);
    glUniform3fv(intensityIndex, 1, glm::value_ptr(mColour));

    GLint ambientCoefficientIndex = program.uniform(ambient);
    glUniform1f(ambientCoefficientIndex, mAmbientCoefficient);

    GLint attenuationIndex = program.uniform(attenuation);
    glUniform1f(attenuationIndex, mAttenuationFactor);
  }

  void Light::print(std::ostream& where) const
  {
    printField(where, "position", toString(mPosition));
    printField(where, ", colour", toString(mColour));
    printField(where, ", ambientCoefficient", mAmbientCoefficient);
    printField(where, ", attenuationFactor", mAttenuationFactor);
  }
}
