// standard libraries
#include <iostream>
#include <algorithm>

// my own
#include <components/controller.hpp>

namespace components
{
  Controller::Controller()
    : Component(CONTROLLER)
  {
  }

  void Controller::createKeyboardAction(const std::string action, Keys keys)
  {
    mActionStates.emplace(action, std::make_pair(0, keys));
  }
  const int Controller::getActionState(const std::string action) const
  {
    auto search = mActionStates.find(action);
    if(search != mActionStates.end())
    {
      return search->second.first;
    }
    return 0;
  }
  void Controller::updateActionStates()
  {
    for( auto &iterator : mActionStates )
    {
      auto& state = iterator.second.first;

      bool keyState = false;
      auto& keys = iterator.second.second;
      for(auto& key : keys)
      {
        keyState = keyState || sf::Keyboard::isKeyPressed(key);
      }
      state = keyState;
    }
  }
  void Controller::addUpdateCallback(UpdateCallback callback)
  {
    mCallbacks.push_back(callback);
  }

  const std::vector< UpdateCallback >& Controller::getUpdateCallbacks() const
  {
    return mCallbacks;
  }

  void Controller::print(std::ostream& where) const
  {
    std::vector<std::string> actions;
    for( auto &i : mActionStates )
    {
      actions.push_back(i.first);
    }

    printVector(where, "actions", actions);
  }
}
