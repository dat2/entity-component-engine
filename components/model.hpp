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

    Model();

    void generate();
    void bind();
    void loadBuffer(std::vector< sf::Vector3f > vertices, std::vector< sf::Vector2f > uvs);
    void unbind();

    void prepareVertexArray(Program &program, std::string vertAttribute, std::string uvAttribute);
    void drawArrays();

  protected:
    virtual void print(std::ostream& where) const;
  private:
    VertexBufferObject mVertexBuffer;
    VertexArrayObject mVertexArray;

    int mNumVertices;
  };
}

#endif
