#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

// libraries
#include <iostream>
#include <glm/glm.hpp>
#include <SFML/System/Vector3.hpp>
#include <json/json.h>

// engine
#include <components/component.hpp>

namespace components
{
  class Transform : public Component
  {
  public:
    Transform(Json::Value value);

    void setPosition(const sf::Vector3f position);
    // void rotate();
    // void scale();

    void updateMatrix();

    const sf::Vector3f& position() const;
    const sf::Vector3f& rotation() const;
    const sf::Vector3f& scale() const;

    const glm::mat4& matrix() const;

  protected:
    virtual void print(std::ostream& where) const;

  private:
    sf::Vector3f mPosition;
    sf::Vector3f mRotation;
    sf::Vector3f mScale;

    glm::mat4 mMatrix;
  };
}

#endif
