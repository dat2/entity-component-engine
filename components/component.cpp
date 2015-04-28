// my own
#include <components/component.hpp>

namespace components
{
  Component::Component(ComponentType type)
    : mType(type)
  { }

  const ComponentType Component::getType() const
  {
    return mType;
  }

  std::ostream& operator<<(std::ostream& os, Component const& c)
  {
    c.print(os);
    return os;
  }

  void Component::print(std::ostream& where) const
  {

  }
}
