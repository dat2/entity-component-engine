#ifndef _CONTROLLER_HPP
#define _CONTROLLER_HPP

// standard libraries
#include <map>
#include <vector>
#include <functional>

// sfml
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Time.hpp>

// my own
#include <components/camera.hpp>

namespace components
{
  class Controller;

  typedef std::function<void(Controller&, Camera&, sf::Time&)> Action;
  typedef std::vector<sf::Keyboard::Key> Keys;

  class Controller : public Component
  {
  public:
    Controller(float moveSpeed, float rotateSpeed);

    void addKeyAction(Keys keys, Action action);

    std::vector< Action > getKeyActions() const;

    const float getMoveSpeed() const;
    const float getRotateSpeed() const;

  protected:
    virtual void print(std::ostream& where) const;

  private:
    std::map< Keys, Action > mKeyActions;

    float mMoveSpeed;
    float mRotateSpeed;
  };

}

#endif
