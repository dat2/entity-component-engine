#ifndef _PHYSICS_HPP
#define _PHYSICS_HPP

// libraries
#include <BulletDynamics/btBulletDynamicsCommon.h>

// engine
#include <systems/system.hpp>

using namespace systems;

namespace physics
{

  class PhysicsSystem : public System
  {
  public:
    PhysicsSystem();
    ~PhysicsSystem();

    void createWorld();

    void entityAdded(engine::Engine& engine, Entity& entity);
    void entityRemoved(engine::Engine& engine, Entity& entity);
    void run(engine::Engine& engine);
  private:
    btBroadphaseInterface* mBroadphase;

    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mDispatcher;

    btSequentialImpulseConstraintSolver* mSolver;

    btDiscreteDynamicsWorld* mWorld;
  };

}

#endif
