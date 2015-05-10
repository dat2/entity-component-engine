#ifndef _MODEL_HPP
#define _MODEL_HPP

// libraries
#include <iostream>
#include <json/json.h>

// engine
#include <assets/render/modelasset.hpp>
#include <components/component.hpp>
#include <entities/entity.hpp>

using namespace components;
using namespace assets;
using namespace entities;

namespace render
{
  class Model : public Component, public Named
  {
  public:
    Model(Json::Value value, EntityPtr entity);

    void draw();
    void setAsset(std::shared_ptr<ModelAsset> asset);

    void print(std::ostream& where) const;
  private:
    std::shared_ptr<ModelAsset> mAsset;
  };
}

#endif
