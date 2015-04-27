#ifndef _COMPONENT_HPP
#define _COMPONENT_HPP

#include <iostream>

namespace components
{
  enum ComponentType
  {
    TRANSFORM   = 1 << 0,
    CAMERA      = 1 << 1,
    CONTROLLER  = 1 << 2,
    MODEL       = 1 << 3,
    TEXTURE     = 1 << 4
  };

  inline ComponentType operator|(ComponentType a, ComponentType b)
  {
    return static_cast<ComponentType>(static_cast<int>(a) | static_cast<int>(b));
  }

  /**
   * Base polymorphic component class.
   */
  class Component
  {
  public:
    Component(ComponentType type);
    const ComponentType getType() const;

    friend std::ostream& operator<<(std::ostream& os, Component const& c);
  protected:
    virtual void print(std::ostream& where) const = 0;
  private:
    ComponentType mType;
  };
}

#endif
