// libraries
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

// my own
#include <components/common/transform.hpp>
#include <utils/utils.hpp>

using namespace utils;

#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)

namespace components
{
  Transform::Transform(Json::Value value)
    : Component(TRANSFORM)
  {
    auto name = value["name"];
    auto pos = value["position"];
    auto rot = value["rotation"];
    auto scale = value["scale"];

    mPosition = sf::Vector3f(pos["x"].asFloat(), pos["y"].asFloat(), pos["z"].asFloat());
    mRotation = sf::Vector3f(rot["x"].asFloat(), rot["y"].asFloat(), rot["z"].asFloat());
    mScale = sf::Vector3f(scale["x"].asFloat(), scale["y"].asFloat(), scale["z"].asFloat());
    updateMatrix();
  }

  void Transform::set(const btTransform &worldTrans)
  {
    mPosition = bulletToSfml(worldTrans.getOrigin());

    auto rot = worldTrans.getRotation();
    mRotation = sf::Vector3f(glm::degrees(rot.x()), glm::degrees(rot.y()), glm::degrees(rot.z()));

    updateMatrix();
  }

  void Transform::setPosition(const sf::Vector3f position)
  {
    mPosition = position;

    updateMatrix();
  }

  void Transform::updateMatrix()
  {
    mMatrix = glm::mat4();

    // translate
    mMatrix = glm::translate(mMatrix, sfmlToGlm(mPosition));

    // rotate
    mMatrix = glm::rotate(mMatrix, glm::radians(mRotation.x), X_AXIS);
    mMatrix = glm::rotate(mMatrix, glm::radians(mRotation.y), Y_AXIS);
    mMatrix = glm::rotate(mMatrix, glm::radians(mRotation.z), Z_AXIS);

    // scale
    mMatrix = glm::scale(mMatrix, sfmlToGlm(mScale));
  }

  const sf::Vector3f& Transform::position() const
  {
    return mPosition;
  }

  const sf::Vector3f& Transform::rotation() const
  {
    return mRotation;
  }

  const sf::Vector3f& Transform::scale() const
  {
    return mScale;
  }

  const glm::mat4& Transform::matrix() const
  {
    return mMatrix;
  }

  void Transform::print(std::ostream& where) const
  {
    printField(where, "position", toString(mPosition));
    printField(where, ", rotation", toString(mRotation));
    printField(where, ", scale", toString(mScale));
  }
}
