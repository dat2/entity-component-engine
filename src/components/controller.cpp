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

  void Controller::createKeyAction(const std::string action, Keys keys)
  {
    mKeyActionStates.emplace(action, std::make_pair(0, keys));
  }
  void Controller::createState(const std::string state)
  {
    mStates[state] = 0;
  }
  const int Controller::getKeyActionState(const std::string action) const
  {
    auto search = mKeyActionStates.find(action);
    if(search != mKeyActionStates.end())
    {
      return search->second.first;
    }
    return 0;
  }
  const int Controller::getState(const std::string state) const
  {
    return mStates.at(state);
  }
  void Controller::updateKeyActionStates()
  {
    for( auto &iterator : mKeyActionStates )
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
  void Controller::updateState(const std::string state, int val)
  {
    mStates[state] = val;
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
    for( auto &i : mKeyActionStates )
    {
      actions.push_back(i.first);
    }

    printVector(where, "actions", actions);
  }
}
