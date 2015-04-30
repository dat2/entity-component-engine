#ifndef _MODEL_ASSET_HPP
#define _MODEL_ASSET_HPP

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

#include <assets/asset.hpp>

#include <assets/rendering/vbo.hpp>
#include <assets/rendering/vao.hpp>
#include <assets/rendering/program.hpp>

namespace assets
{

  class ModelAsset : public Asset
  {
  public:
    ModelAsset(const std::string name, const std::string filepath);

    void generate();
    void bind();
    void unbind();

    void loadBuffer(const std::vector< sf::Vector3f > vertices, const std::vector< sf::Vector2f > uvs);
    void prepareVAO(Program &program, const std::string vertAttribute, const std::string uvAttribute);

    void draw();

    const int getNumVertices() const;
    void print(std::ostream& where) const;
  private:
    VertexBufferObject mVertexBuffer;
    VertexArrayObject mVertexArray;

    int mNumVertices;
  };

}

#endif
