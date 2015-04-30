// standard libraries
#include <iostream>

// sfml
#include <SFML/System/Vector3.hpp>

// my own
#include <components/transform.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace components
{
  Transform::Transform(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale)
    : Component(TRANSFORM), mPosition(position), mRotation(rotation), mScale(scale)
  {
  }

  void Transform::move(const sf::Vector3f position)
  {
    mPosition += position;
  }

  const sf::Vector3f& Transform::getPosition() const
  {
    return mPosition;
  }

  const sf::Vector3f& Transform::getRotation() const
  {
    return mRotation;
  }

  const sf::Vector3f& Transform::getScale() const
  {
    return mScale;
  }

  void Transform::print(std::ostream& where) const
  {
    printField(where, "position", toString(mPosition));
    printField(where, ", rotation", toString(mRotation));
    printField(where, ", scale", toString(mScale));
  }
}
