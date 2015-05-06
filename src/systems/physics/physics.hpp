#ifndef _PHYSICS_HPP
#define _PHYSICS_HPP

// libraries
#include <BulletDynamics/btBulletDynamicsCommon.h>

// engine
#include <systems/system.hpp>

namespace systems
{

  class PhysicsSystem : public System
  {
  public:
    PhysicsSystem();
    ~PhysicsSystem();

    void createWorld();

    void fillWorld();

    void run(engine::Engine& engine);
  private:
    btBroadphaseInterface* mBroadphase;

    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mDispatcher;

    btSequentialImpulseConstraintSolver* mSolver;

    btDiscreteDynamicsWorld* mWorld;

    // TODO delete
    btCollisionShape* mGroundShape;
    btRigidBody* mGroundRigidBody;

    btCollisionShape* mFallShape;
    btRigidBody* mFallRigidBody;
  };

}

#endif
