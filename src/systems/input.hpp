#ifndef _INPUT_HPP
#define _INPUT_HPP

// libraries
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Window.hpp>

// engine
#include <systems/system.hpp>

namespace systems
{
  class InputSystem : public System
  {
  public:
    InputSystem(sf::Window& window);

    void run(engine::Engine& engine);

  private:
    sf::Clock clock;

    sf::Window& mWindow;
  };
}
#endif
