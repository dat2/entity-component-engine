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
  InputSystem::~InputSystem()
  {
    mWindow.close();
  }

  void InputSystem::run(engine::Engine& engine)
  {
    auto elapsed = engine.getElapsed();
    for( auto &kv : mEntities )
    {
      auto entity = kv.second;
      auto controller = entity->getComponent<Controller>();

      if(mWindow.hasFocus())
      {
        controller->updateKeyActionStates();
      }

      auto callbacks = controller->getUpdateCallbacks();
      for(auto &callback : callbacks)
      {
        callback(engine, *entity, mWindow, elapsed);
      }
    }
  }
}
