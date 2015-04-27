// standard libraries
#include <iostream>

// glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/compatibility.hpp>

// my own
#include <components/camera.hpp>
#include <utils/utils.hpp>

using namespace utils;

#define CAMERA_V glm::vec3(0,1,0)
#define CAMERA_H glm::vec3(1,0,0)
#define CAMERA_N glm::vec3(0,0,-1)
;

namespace components
{
  Camera::Camera(sf::Vector3f pos, float fov, float ratio, float near, float far)
    : Component(CAMERA),
      mPosition(sfmlToGlm(pos)), mFieldOfView(fov), mAspectRatio(ratio), mNear(near), mFar(far),
      mVerticalAngle(0.0f), mHorizontalAngle(0.0f)
  {
  }

  void Camera::move(const sf::Vector3f diff)
  {
    mPosition += sfmlToGlm(diff);
  }

  void Camera::rotate(float vertical, float horizontal)
  {
    mVerticalAngle += glm::radians(vertical);
    mHorizontalAngle += glm::radians(horizontal);
  }

  void Camera::lookAt(sf::Vector3f pos)
  {
    glm::vec3 dir = glm::normalize(sfmlToGlm(pos) - mPosition);
    mVerticalAngle = glm::asin(-dir.y);
    mHorizontalAngle = -glm::atan(-dir.x, -dir.z);
  }

  const glm::mat4 Camera::mwv() const
  {
    return mcv() * mwc();
  }

  const glm::mat4 Camera::mcv() const
  {
    return glm::perspective(glm::radians(mFieldOfView), mAspectRatio, mNear, mFar);
  }

  const glm::mat4 Camera::mwc() const
  {
    return glm::translate(orientation(), -mPosition);
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
    where << "Camera("
      << "position=" << mPosition.x << "," << mPosition.y << "," << mPosition.z << ")"
      << ", fov=" << mFieldOfView
      << ", aspectRatio=" << mAspectRatio
      << ", near=" << mNear
      << ", far=" << mFar
      << ", verticalAngle=" << glm::degrees(mVerticalAngle)
      << ", horizontalAngle=" << glm::degrees(mHorizontalAngle)
      << ")";
  }
}
