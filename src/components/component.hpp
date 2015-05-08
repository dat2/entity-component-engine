#ifndef _COMPONENT_HPP
#define _COMPONENT_HPP

// libraries
#include <iostream>
#include <algorithm>

// engine
#include <misc/printable.hpp>

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
    TEXTURE     = 1 << 4,
    LIGHT       = 1 << 5,
    RIGIDBODY   = 1 << 6
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
      case TRANSFORM: return "Transform";
      case CAMERA: return "Camera";
      case CONTROLLER: return "Controller";
      case MODEL: return "Model";
      case TEXTURE: return "Texture";
      case LIGHT: return "Light";
      case RIGIDBODY: return "RigidBody";
      default: return "";
    }
  }

  /**
   * Base polymorphic component class. You must define a "print" method, so as
   * to make debugging easier.
   */
  class Component : public Printable
  {
  public:
    Component(ComponentType type);
    const ComponentType getType() const;

  private:
    ComponentType mType;
  };

  typedef std::shared_ptr<Component> ComponentPtr;
}

#endif
