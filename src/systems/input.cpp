// libraries
#include <SFML/System/Time.hpp>

// engine
#include <components/controller.hpp>
#include <systems/input.hpp>

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
      auto controller = entity.getComponent<Controller>();

      if(mWindow.hasFocus())
      {
        controller->updateKeyActionStates();
      }

      auto callbacks = controller->getUpdateCallbacks();
      for(auto &callback : callbacks)
      {
        callback(engine, e, *controller, mWindow, elapsed);
      }
    }
  }
}
