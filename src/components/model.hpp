#ifndef _MODEL_HPP
#define _MODEL_HPP

// standard libraries
#include <iostream>

// my own
#include <components/component.hpp>
#include <assets/modelasset.hpp>

#include <json/json.h>

using namespace assets;

namespace components
{
  class Model : public Component, public Named
  {
  public:
    Model(const std::string name);
    Model(Json::Value value);

    void draw();
    void setAsset(std::shared_ptr<ModelAsset> asset);

    void print(std::ostream& where) const;
  private:
    std::shared_ptr<ModelAsset> mAsset;
  };
}

#endif
