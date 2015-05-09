//libraries
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// engine
#include <components/render/camera.hpp>
#include <misc/printable.hpp>
#include <utils/utils.hpp>

using namespace utils;

#define CAMERA_V glm::vec3(0,1,0)
#define CAMERA_H glm::vec3(1,0,0)
#define CAMERA_N glm::vec3(0,0,-1)
;

namespace render
{
  Camera::Camera(Json::Value value)
    : Component(CAMERA),
      mFieldOfView(value["fov"].asFloat()), mAspectRatio(value["aspect"].asFloat()),
      mNear(value["near"].asFloat()), mFar(value["far"].asFloat()),
      mVerticalAngle(0.0f), mHorizontalAngle(0.0f), mUpdated(false)
  {
    auto lookAtPos = value["lookAt"];
    mLookAtPos = glm::vec3(lookAtPos["x"].asFloat(), lookAtPos["y"].asFloat(), lookAtPos["z"].asFloat());
  }

  void Camera::rotate(float vertical, float horizontal)
  {
    mVerticalAngle += glm::radians(vertical);
    mHorizontalAngle += glm::radians(horizontal);
    mUpdated = true;
  }

  void Camera::lookFrom(const glm::vec3 pos)
  {
    glm::vec3 dir = glm::normalize(mLookAtPos - pos);
    mVerticalAngle = glm::asin(-dir.y);
    mHorizontalAngle = -glm::atan(-dir.x, -dir.z);
    mUpdated = true;
  }

  bool Camera::isUpdated()
  {
    if(mUpdated)
    {
      mUpdated = false;
      return true;
    }
    return mUpdated;
  }

  const glm::mat4 Camera::mwv(const glm::vec3& position) const
  {
    return mcv() * mwc(position);
  }

  const glm::mat4 Camera::mcv() const
  {
    return glm::perspective(glm::radians(mFieldOfView), mAspectRatio, mNear, mFar);
  }

  const glm::mat4 Camera::mwc(const glm::vec3& position) const
  {
    return glm::translate(orientation(), -position);
  }

  const glm::mat4 Camera::orientation() const
  {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, mVerticalAngle, CAMERA_H);
    orientation = glm::rotate(orientation, mHorizontalAngle, CAMERA_V);
    return orientation;
  }

  const glm::vec3 Camera::forward() const
  {
    return glm::vec3(glm::inverse(orientation()) * glm::vec4(CAMERA_N, 1.0));
  }

  const glm::vec3 Camera::up() const
  {
    return glm::vec3(glm::inverse(orientation()) * glm::vec4(CAMERA_V, 1.0));
  }

  const glm::vec3 Camera::left() const
  {
    return glm::vec3(glm::inverse(orientation()) * glm::vec4(CAMERA_H, 1.0));
  }

  void Camera::print(std::ostream& where) const
  {
    printField(where, "fov", mFieldOfView);
    printField(where, ", aspectRatio", mAspectRatio);
    printField(where, ", near", mNear);
    printField(where, ", far", mFar);
    printField(where, ", verticalAngle", glm::degrees(mVerticalAngle));
    printField(where, ", horizontalAngle", glm::degrees(mHorizontalAngle));
  }
}
