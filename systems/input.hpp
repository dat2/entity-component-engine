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
    InputSystem(Camera& camera)
    : System(CAMERA | CONTROLLER), mCamera(camera) {}

    void handle(Controller &controller);

  private:
    Camera& mCamera;
    sf::Clock clock;
  };
}
#endif
