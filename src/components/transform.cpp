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

  Transform::Transform(Json::Value value)
    : Component(TRANSFORM)
  {
    auto name = value["name"];
    auto pos = value["position"];
    auto rot = value["rotation"];
    auto scale = value["scale"];

    mPosition = sf::Vector3f(pos["x"].asFloat(), pos["y"].asFloat(), pos["z"].asFloat());
    mRotation = sf::Vector3f(rot["x"].asFloat(), rot["y"].asFloat(), rot["z"].asFloat());
    mScale = sf::Vector3f(scale["x"].asFloat(), scale["y"].asFloat(), scale["z"].asFloat());
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
