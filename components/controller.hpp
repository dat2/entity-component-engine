#ifndef _CONTROLLER_HPP
#define _CONTROLLER_HPP

// standard libraries
#include <unordered_map>
#include <vector>
#include <functional>

// sfml
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Time.hpp>

// my own
#include <entities/entity.hpp>

using namespace entities;

namespace components
{
  class Controller;

  typedef std::function<void(Entity&, Controller&, sf::Window&, sf::Time&)> UpdateCallback;
  typedef std::vector<sf::Keyboard::Key> Keys;

  class Controller : public Component
  {
  public:
    Controller();

    void createKeyboardAction(const std::string action, Keys keys);
    const int getActionState(const std::string action) const;
    void updateActionStates();
    void addUpdateCallback(UpdateCallback callback);
    const std::vector< UpdateCallback >& getUpdateCallbacks() const;

  protected:
    virtual void print(std::ostream& where) const;

  private:
    std::unordered_map< std::string, std::pair<int, Keys> > mActionStates;

    std::vector< UpdateCallback > mCallbacks;
  };

}

#endif
