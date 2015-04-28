#ifndef _MODEL_HPP
#define _MODEL_HPP

// standard libraries
#include <vector>
#include <iostream>

// sfml
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

// my own
#include <utils/rendering/vbo.hpp>
#include <utils/rendering/vao.hpp>
#include <utils/rendering/program.hpp>
#include <components/component.hpp>

using namespace utils;

namespace components
{
  class Model : public Component
  {
  public:

    Model(const std::string name, std::vector< sf::Vector3f > vertices, std::vector< sf::Vector2f > uvs);

    const std::string getName() const;

    void generate();
    void bind();
    void loadBuffer();
    void unbind();

    void prepareVAO(Program &program, std::string vertAttribute, std::string uvAttribute);
    void draw();

  protected:
    virtual void print(std::ostream& where) const;
  private:
    const std::string mName;
    std::vector< sf::Vector3f > mVertices;
    std::vector< sf::Vector2f > mUvs;

    VertexBufferObject mVertexBuffer;
    VertexArrayObject mVertexArray;

    int mNumVertices;
  };
}

#endif
