#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

// my own
#include <components/component.hpp>

using namespace components;

namespace systems
{
  class System
  {
  public:
    System(ComponentType requiredTypes)
      : mRequiredTypes(requiredTypes) { }

    bool hasTypes(ComponentType type) const;
  private:
    ComponentType mRequiredTypes;
  };
}

#endif
