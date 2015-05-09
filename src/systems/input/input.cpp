// libraries
#include <SFML/System/Time.hpp>

// engine
#include <components/input/controller.hpp>
#include <engine/engine.hpp>
#include <systems/input/input.hpp>

using namespace components;

namespace input
{
  InputSystem::InputSystem(sf::Window& window)
    : System("input", CONTROLLER), mWindow(window)
  {
  }

  void InputSystem::run(engine::Engine& engine)
  {
    auto elapsed = engine.getElapsed();
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
        callback(engine, e, mWindow, elapsed);
      }
    }
  }
}
