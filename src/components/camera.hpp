#ifndef _CAMERA_HPP
#define _CAMERA_HPP

//libraries
#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <SFML/System/Vector3.hpp>

// engine
#include <components/component.hpp>
#include <misc/printable.hpp>

namespace components
{
  class Camera : public Component
  {
  public:
    Camera(sf::Vector3f pos, float fov, float ratio, float near, float far);

    void move(const glm::vec3 diff);
    void rotate(float vertical, float horizontal);
    void lookAt(sf::Vector3f pos);

    const bool needsUpdate() const;
    void update();

    const glm::mat4 mwv() const;
    const glm::mat4 mcv() const;
    const glm::mat4 mwc() const;

    const glm::mat4 orientation() const;

    const glm::vec3 forward() const;
    const glm::vec3 up() const;
    const glm::vec3 left() const;

    const glm::vec3 position() const;

    void print(std::ostream& where) const;

  private:
    glm::vec3 mPosition;

    float mFieldOfView;
    float mAspectRatio;
    float mNear;
    float mFar;

    float mVerticalAngle;
    float mHorizontalAngle;

    bool mNeedsUpdate;
  };
}

#endif
