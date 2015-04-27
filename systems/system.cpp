#include <systems/system.hpp>

namespace systems
{
  bool System::hasTypes(ComponentType type) const
  {
    return mRequiredTypes == type;
  }
}
