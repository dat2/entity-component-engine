#ifndef _INPUT_HPP
#define _INPUT_HPP

// sfml
#include <SFML/Window/Window.hpp>
#include <SFML/System/Clock.hpp>

// my own
#include <systems/system.hpp>

namespace systems
{
  class InputSystem : public System
  {
  public:
    InputSystem(sf::Window& window);

    void run();

  private:
    sf::Clock clock;

    sf::Window& mWindow;
  };
}
#endif
