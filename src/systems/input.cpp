// sfml
#include <SFML/System/Time.hpp>

// my own
#include <systems/input.hpp>

#include <components/controller.hpp>

using namespace components;

namespace systems
{
  InputSystem::InputSystem(sf::Window& window)
    : System("input", CONTROLLER), mWindow(window)
  {
  }

  void InputSystem::run()
  {
    sf::Time elapsed = clock.restart();

    for( auto &e : mEntities )
    {
      auto entity = e.get();
      auto controller = entity.getComponent<Controller>(CONTROLLER);

      controller->updateActionStates();

      auto callbacks = controller->getUpdateCallbacks();
      for(auto &callback : callbacks)
      {
        callback(e, *controller, mWindow, elapsed);
      }
    }
  }
}
