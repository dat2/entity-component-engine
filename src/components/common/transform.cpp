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
  Transform::Transform(Json::Value value, EntityPtr entity)
    : Component(TRANSFORM), mUpdated(false)
  {
    auto name = value["name"];

    mPosition = jsonToVector(value, "position");
    mRotation = jsonToVector(value, "rotation");
    mScale = jsonToVector(value, "scale");
    updateMatrix();
  }

  void Transform::set(const btTransform &worldTrans)
  {
    mPosition = bulletToGlm(worldTrans.getOrigin());

    auto rot = worldTrans.getRotation();
    mRotation = glm::vec3(glm::degrees(rot.x()), glm::degrees(rot.y()), glm::degrees(rot.z()));

    updateMatrix();
  }

  void Transform::setPosition(const glm::vec3 position)
  {
    mPosition = position;

    updateMatrix();
  }

  void Transform::move(const glm::vec3 diff)
  {
    mPosition += diff;

    updateMatrix();
  }

  void Transform::updateMatrix()
  {
    mMatrix = glm::mat4();

    // translate
    mMatrix = glm::translate(mMatrix, mPosition);

    // rotate
    mMatrix = glm::rotate(mMatrix, glm::radians(mRotation.x), X_AXIS);
    mMatrix = glm::rotate(mMatrix, glm::radians(mRotation.y), Y_AXIS);
    mMatrix = glm::rotate(mMatrix, glm::radians(mRotation.z), Z_AXIS);

    // scale
    mMatrix = glm::scale(mMatrix, mScale);
    mUpdated = true;
  }

  bool Transform::isUpdated()
  {
    if(mUpdated)
    {
      mUpdated = false;
      return true;
    }
    return mUpdated;
  }

  const glm::vec3& Transform::position() const
  {
    return mPosition;
  }

  const glm::vec3& Transform::rotation() const
  {
    return mRotation;
  }

  const glm::vec3& Transform::scale() const
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
