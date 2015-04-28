#ifndef _INPUT_HPP
#define _INPUT_HPP

// sfml
#include <SFML/System/Clock.hpp>

// my own
#include <components/controller.hpp>
#include <components/camera.hpp>
#include <systems/system.hpp>

using namespace components;

namespace systems
{
  class InputSystem : public System
  {
  public:
    InputSystem();

    void entityAdded(Entity& entity);
    void run();

  private:
    sf::Clock clock;

    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<Controller> mController;
  };
}
#endif
