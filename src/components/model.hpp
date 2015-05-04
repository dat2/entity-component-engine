#ifndef _MODEL_HPP
#define _MODEL_HPP

// libraries
#include <iostream>
#include <json/json.h>

// engine
#include <assets/modelasset.hpp>
#include <components/component.hpp>

using namespace assets;

namespace components
{
  class Model : public Component, public Named
  {
  public:
    Model(Json::Value value);

    void draw();
    void setAsset(std::shared_ptr<ModelAsset> asset);

    void print(std::ostream& where) const;
  private:
    std::shared_ptr<ModelAsset> mAsset;
  };
}

#endif
