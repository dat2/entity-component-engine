// sfml
#include <SFML/System/Time.hpp>

// my own
#include <systems/input.hpp>

namespace systems
{
  InputSystem::InputSystem()
    : System(CAMERA | CONTROLLER), mCamera(nullptr), mController(nullptr)
  {
  }

  void InputSystem::entityAdded(Entity& entity)
  {
    mCamera = std::dynamic_pointer_cast<Camera>(entity.getComponent(CAMERA));
    mController = std::dynamic_pointer_cast<Controller>(entity.getComponent(CONTROLLER));
  }

  void InputSystem::run()
  {
    sf::Time elapsed = clock.restart();
    if(mCamera != nullptr && mController != nullptr)
    {
      auto actions = mController->getKeyActions();
      for( auto &action : actions )
      {
        action(*mController, *mCamera, elapsed);
      }
    }
  }
}
