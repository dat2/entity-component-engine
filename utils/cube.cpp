// standard libraries
#include <iostream>

// my own
#include <utils/cube.hpp>

namespace utils
{
  std::vector< sf::Vector3f > cubeVertices()
  {
    sf::Vector3f v0(-0.5f, -0.5f,  0.5f); // bottom left
    sf::Vector3f v1( 0.5f, -0.5f,  0.5f); // bottom right
    sf::Vector3f v2(-0.5f,  0.5f,  0.5f); // top left
    sf::Vector3f v3( 0.5f,  0.5f,  0.5f); // top right

    sf::Vector3f v4(-0.5f, -0.5f, -0.5f); // bottom left
    sf::Vector3f v5( 0.5f, -0.5f, -0.5f); // bottom right
    sf::Vector3f v6(-0.5f,  0.5f, -0.5f); // top left
    sf::Vector3f v7( 0.5f,  0.5f, -0.5f); // top right

    return {
      // front face
      v0, v1, v2, v2, v1, v3,

      // back face
      v4, v5, v6, v6, v5, v7,

      // left face
      v4, v0, v6, v6, v0, v2,

      // right face
      v1, v5, v3, v3, v5, v7,

      // top face
      v2, v3, v6, v6, v3, v7,

      // bottom face
      v4, v5, v0, v0, v5, v1
    };
  }

  std::vector< sf::Vector2f > cubeUVs()
  {
    sf::Vector2f uv0( 0.0f, 0.0f); // bottom left
    sf::Vector2f uv1( 1.0f, 0.0f); // bottom right
    sf::Vector2f uv2( 0.0f, 1.0f); // top left
    sf::Vector2f uv3( 1.0f, 1.0f); // top right

    return {
      // front face
      uv0, uv1, uv2, uv2, uv1, uv3,

      // back face
      uv0, uv1, uv2, uv2, uv1, uv3,

      // left face
      uv0, uv1, uv2, uv2, uv1, uv3,

      // right face
      uv0, uv1, uv2, uv2, uv1, uv3,

      // top face
      uv0, uv1, uv2, uv2, uv1, uv3,

      // bottom face
      uv0, uv1, uv2, uv2, uv1, uv3,
    };
  }
}
