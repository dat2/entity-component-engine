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

  void InputSystem::run(engine::Engine& engine)
  {
    sf::Time elapsed = clock.restart();

    for( auto &e : mEntities )
    {
      auto entity = e.get();
      auto controller = entity.getComponent<Controller>(CONTROLLER);

      controller->updateKeyActionStates();

      auto callbacks = controller->getUpdateCallbacks();
      for(auto &callback : callbacks)
      {
        callback(engine, e, *controller, mWindow, elapsed);
      }
    }
  }
}
