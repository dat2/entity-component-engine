// libraries
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// engine
#include <components/physics/rigidbody.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace physics
{
  RigidBody::RigidBody(Json::Value value)
    : Component(RIGIDBODY),
    mShapeType(value["shape"].asString()), mMass(value["mass"].asFloat()),
    mRadius(value["radius"].asFloat()),
    mShape(nullptr), mRigidBody(nullptr)
  {
  }

  RigidBody::~RigidBody()
  {
    if(mRigidBody != nullptr && mShape != nullptr)
    {
      delete mRigidBody->getMotionState();
      delete mRigidBody;
      delete mShape;
    }
  }

  const btTransform* RigidBody::getWorldTransform() const
  {
    if(mRigidBody == nullptr)
    {
      return nullptr;
    }
    auto rtn = new btTransform;
    mRigidBody->getMotionState()->getWorldTransform(*rtn);
    return rtn;
  }

  void RigidBody::setTransform(Transform& transform)
  {
    auto& scale = transform.scale();
    auto& position = transform.position();

    auto& rotation = transform.rotation();
    auto rotationQ = btQuaternion(rotation.x,rotation.y,rotation.z,1.0);
    auto rotationMatrix = btMatrix3x3(rotationQ);

    if(mShapeType == "splane")
    {
      // TODO check
      auto normal = btVector3(0, 1, 0);
      auto rotatedNormal = normal.dot3(rotationMatrix.getRow(0),rotationMatrix.getRow(1),rotationMatrix.getRow(2));
      mShape = new btStaticPlaneShape(
        rotatedNormal,
        rotatedNormal.length()
      );
    }
    else if(mShapeType == "box")
    {
      mShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
      mShape->setLocalScaling(glmToBullet(scale));
    }
    else if(mShapeType == "sphere")
    {
      mShape = new btSphereShape(mRadius);
    }

    mState = new btDefaultMotionState(btTransform(rotationQ, glmToBullet(position)));
  }

  void RigidBody::createRigidBody()
  {
    btVector3 inertia(0, 0, 0);
    mShape->calculateLocalInertia(mMass, inertia);

    btRigidBody::btRigidBodyConstructionInfo
      rigidBodyCI(mMass, mState, mShape, inertia);
    mRigidBody = new btRigidBody(rigidBodyCI);
  }

  void RigidBody::addToWorld(btDiscreteDynamicsWorld* world)
  {
    if(world != nullptr && mRigidBody != nullptr)
    {
      world->addRigidBody(mRigidBody);
    }
  }
  void RigidBody::removeFromWorld(btDiscreteDynamicsWorld* world)
  {
    if(world != nullptr)
    {
      world->removeRigidBody(mRigidBody);
    }
  }

  void RigidBody::print(std::ostream& where) const
  {

  }
}
