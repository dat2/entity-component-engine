// my own
#include <components/component.hpp>

namespace components
{
  Component::Component(ComponentType type)
    : Printable(ToString(type)), mType(type)
  { }

  const ComponentType Component::getType() const
  {
    return mType;
  }
}
