#ifndef _CAMERA_HPP
#define _CAMERA_HPP

//libraries
#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <SFML/System/Vector3.hpp>
#include <json/json.h>

// engine
#include <components/component.hpp>
#include <misc/printable.hpp>

using namespace components;

namespace render
{
  class Camera : public Component
  {
  public:
    Camera(Json::Value value);

    void rotate(float vertical, float horizontal);
    void lookFrom(const glm::vec3 pos);

    bool isUpdated();

    const glm::mat4 mwv(const glm::vec3& position) const;
    const glm::mat4 mcv() const;
    const glm::mat4 mwc(const glm::vec3& position) const;

    const glm::mat4 orientation() const;

    const glm::vec3 forward() const;
    const glm::vec3 up() const;
    const glm::vec3 left() const;

    void print(std::ostream& where) const;

  private:
    glm::vec3 mLookAtPos;

    float mFieldOfView;
    float mAspectRatio;
    float mNear;
    float mFar;

    float mVerticalAngle;
    float mHorizontalAngle;

    bool mUpdated;
  };
}

#endif
