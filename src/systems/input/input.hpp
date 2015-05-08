#ifndef _INPUT_HPP
#define _INPUT_HPP

// libraries
#include <SFML/Window/Window.hpp>

// engine
#include <systems/system.hpp>

using namespace systems;

namespace input
{
  class InputSystem : public System
  {
  public:
    InputSystem(sf::Window& window);

    void run(engine::Engine& engine);

  private:

    sf::Window& mWindow;
  };
}
#endif
