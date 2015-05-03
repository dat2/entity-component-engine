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

namespace engine
{
  class Engine;
}

namespace components
{
  class Controller;

  typedef std::function<void(engine::Engine&, Entity&, Controller&, sf::Window&, sf::Time&)> UpdateCallback;
  typedef std::vector<sf::Keyboard::Key> Keys;

  class Controller : public Component
  {
  public:
    Controller();

    void createKeyAction(const std::string action, Keys keys);
    void createState(const std::string state);

    const int getKeyActionState(const std::string action) const;
    const int getState(const std::string state) const;

    void updateKeyActionStates();
    void updateState(const std::string state, int val);

    void addUpdateCallback(UpdateCallback callback);
    const std::vector< UpdateCallback >& getUpdateCallbacks() const;

  protected:
    virtual void print(std::ostream& where) const;

  private:
    std::unordered_map< std::string, std::pair<int, Keys> > mKeyActionStates;
    std::unordered_map<std::string, int> mStates;

    std::vector< UpdateCallback > mCallbacks;
  };

}

#endif
