#ifndef _PHYSICS_C_HPP
#define _PHYSICS_C_HPP

//libraries
#include <json/json.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>

//engine
#include <components/component.hpp>
#include <components/common/transform.hpp>
#include <entities/entity.hpp>

using namespace components;
using namespace entities;

namespace physics
{

  class RigidBody : public Component
  {
  public:
    RigidBody(Json::Value value, EntityPtr entity);
    ~RigidBody();

    const btTransform* getWorldTransform() const;
    void setTransform(Transform& transform);

    void createRigidBody();

    void addToWorld(btDiscreteDynamicsWorld* world);
    void removeFromWorld(btDiscreteDynamicsWorld* world);

    void applyForce(const glm::vec3 force, const glm::vec3 position = glm::vec3());

    void print(std::ostream& where) const;
  private:
    const std::string mShapeType;
    btScalar mMass;
    btScalar mRadius;

    btCollisionShape* mShape;
    btMotionState* mState;

    btRigidBody* mRigidBody;
  };

}


#endif
