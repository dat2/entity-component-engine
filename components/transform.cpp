// standard libraries
#include <iostream>

// sfml
#include <SFML/System/Vector3.hpp>

// my own
#include <components/transform.hpp>

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
    where << "Transform("
      << "position=(" << mPosition.x << "," << mPosition.y << "," << mPosition.z << ")"
      << ", rotation=(" << mRotation.x << "," << mRotation.y << "," << mRotation.z << ")"
      << ", scale=(" << mScale.x << "," << mScale.y << "," << mScale.z << ")"
      << ")";
  }
}
