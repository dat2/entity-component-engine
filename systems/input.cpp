// sfml
#include <SFML/System/Time.hpp>

// my own
#include <systems/input.hpp>

namespace systems
{
  void InputSystem::handle(Controller &controller)
  {
    sf::Time elapsed = clock.restart();
    auto actions = controller.getKeyActions();
    for( auto &action : actions )
    {
      action(controller, mCamera, elapsed);
    }
  }
}
