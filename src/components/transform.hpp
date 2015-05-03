#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

#include <iostream>
#include <SFML/System/Vector3.hpp>

#include <glm/glm.hpp>

#include <components/component.hpp>

#include <json/json.h>

namespace components
{
  class Transform : public Component
  {
  public:
    Transform(const sf::Vector3f position, const sf::Vector3f rotation, const sf::Vector3f scale);
    Transform(Json::Value value);

    void move(const sf::Vector3f position);
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
