#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

#include <iostream>
#include <SFML/System/Vector3.hpp>

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

    const sf::Vector3f& getPosition() const;
    const sf::Vector3f& getRotation() const;
    const sf::Vector3f& getScale() const;

  protected:
    virtual void print(std::ostream& where) const;

  private:
    sf::Vector3f mPosition;
    sf::Vector3f mRotation;
    sf::Vector3f mScale;
  };
}

#endif
