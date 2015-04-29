#ifndef _COMPONENT_HPP
#define _COMPONENT_HPP

#include <iostream>
#include <functional>

namespace components
{

  // this allows systems to have a bit mask of components.
  enum ComponentType
  {
    EMPTY,
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
  inline ComponentType operator&(ComponentType a, ComponentType b)
  {
    return static_cast<ComponentType>(static_cast<int>(a) & static_cast<int>(b));
  }
  inline const char* ToString(ComponentType a)
  {
    switch(a)
    {
      case TRANSFORM: return "TRANSFORM";
      case CAMERA: return "CAMERA";
      case CONTROLLER: return "CONTROLLER";
      case MODEL: return "MODEL";
      case TEXTURE: return "TEXTURE";
      default: return "";
    }
  }

  /**
   * Base polymorphic component class. You must define a "print" method, so as
   * to make debugging easier.
   */
  class Component
  {
  public:
    Component(ComponentType type);
    const ComponentType getType() const;

    friend std::ostream& operator<<(std::ostream& os, Component const& c);
  protected:
    virtual void print(std::ostream& where) const;
  private:
    ComponentType mType;
  };

  typedef std::shared_ptr<Component> ComponentPtr;
}

#endif
