// standard libraries
#include <iostream>
#include <algorithm>

// my own
#include <components/controller.hpp>

namespace components
{
  Controller::Controller(float moveSpeed, float rotateSpeed)
    : Component(CONTROLLER), mMoveSpeed(moveSpeed), mRotateSpeed(rotateSpeed)
  {
  }

  void  Controller::addKeyAction(Keys keys, Action action)
  {
    mKeyActions.insert(std::pair<Keys, Action>(keys, action));
  }

  std::vector< Action > Controller::getKeyActions() const
  {
    std::vector< Action > actions;

    for( auto &pair : mKeyActions )
    {
      auto keys = pair.first;
      if(std::any_of(keys.begin(), keys.end(),
        [](sf::Keyboard::Key key) { return sf::Keyboard::isKeyPressed(key); }))
      {
        actions.push_back(pair.second);
      }
    }

    return actions;
  }

  const float Controller::getMoveSpeed() const
  {
    return mMoveSpeed;
  }

  const float Controller::getRotateSpeed() const
  {
    return mRotateSpeed;
  }

  void Controller::print(std::ostream& where) const
  {
    where << "Controller("
      << "moveSpeed=" << mMoveSpeed
      << ", rotateSpeed=" << mRotateSpeed
      << ")";
  }
}
