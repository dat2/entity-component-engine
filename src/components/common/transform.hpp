#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

// libraries
#include <iostream>
#include <glm/glm.hpp>
#include <json/json.h>
#include <LinearMath/btTransform.h>

// engine
#include <components/component.hpp>
#include <entities/entity.hpp>

using namespace entities;

namespace components
{
  class Transform : public Component
  {
  public:
    Transform(Json::Value value, EntityPtr entity);

    void set(const btTransform &worldTrans);

    void setPosition(const glm::vec3 position);
    // void rotate();
    // void scale();

    void move(const glm::vec3 diff);

    void updateMatrix();
    bool isUpdated();

    const glm::vec3& position() const;
    const glm::vec3& rotation() const;
    const glm::vec3& scale() const;

    const glm::mat4& matrix() const;

  protected:
    virtual void print(std::ostream& where) const;

  private:
    glm::vec3 mPosition;
    glm::vec3 mRotation;
    glm::vec3 mScale;

    glm::mat4 mMatrix;
    bool mUpdated;
  };
}

#endif
