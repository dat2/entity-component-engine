#ifndef _MODEL_ASSET_HPP
#define _MODEL_ASSET_HPP

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

#include <assets/asset.hpp>

#include <assets/rendering/vbo.hpp>
#include <assets/rendering/vao.hpp>
#include <assets/rendering/program.hpp>

#include <tiny_obj_loader.h>

namespace assets
{

  class ModelAsset : public Asset
  {
  public:
    ModelAsset(const std::string name, const std::string filepath);

    void generate();
    void bind();
    void unbind();

    void loadBuffer();
    void prepareVAO(Program &program, const std::string vertAttribute, const std::string uvAttribute, const std::string normalAttribute);

    void draw();
    void load();

    const int getNumVertices() const;
    void print(std::ostream& where) const;
  private:
    VertexBufferObject mVertexBuffer;
    VertexArrayObject mVertexArray;

    std::vector<tinyobj::shape_t> mShapes;
    std::vector<tinyobj::material_t> mMaterials;

    int mNumVertices;
  };

}

#endif
