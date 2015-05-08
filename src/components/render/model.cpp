// libraries
#include <iostream>

// engine
#include <components/render/model.hpp>

namespace render
{
  Model::Model(Json::Value value)
    : Component(MODEL), Named(value["name"].asString()), mAsset(nullptr)
  {
  }

  void Model::draw()
  {
    if(mAsset)
    {
      mAsset->draw();
    }
  }

  void Model::setAsset(std::shared_ptr<ModelAsset> asset)
  {
    mAsset = asset;
  }

  void Model::print(std::ostream& where) const
  {
    where << "asset=(";
    if(mAsset)
    {
      mAsset->print(where);
    }
    where << ")";
  }
}
