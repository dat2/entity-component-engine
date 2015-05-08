// libraries

// engine
#include <components/common/transform.hpp>
#include <components/physics/rigidbody.hpp>
#include <engine/engine.hpp>
#include <systems/physics/physics.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace physics
{
  PhysicsSystem::PhysicsSystem()
    : System("PhysicsSystem", TRANSFORM | RIGIDBODY),
    mBroadphase(nullptr), mCollisionConfiguration(nullptr), mDispatcher(nullptr),
    mSolver(nullptr), mWorld(nullptr)
  {
    createWorld();
  }

  PhysicsSystem::~PhysicsSystem()
  {
    delete mWorld;
    delete mSolver;
    delete mDispatcher;
    delete mCollisionConfiguration;
    delete mBroadphase;
  }

  void PhysicsSystem::createWorld()
  {
    // build the broadphase algorithm
    mBroadphase = new btDbvtBroadphase();

    // build the collision config and dispatcher
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

    // build the physics solver
    mSolver = new btSequentialImpulseConstraintSolver;

    // make the world
    mWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
    mWorld->setGravity(btVector3(0, -10, 0));
  }

  void PhysicsSystem::entityAdded(engine::Engine& engine, Entity& entity)
  {
    auto t = entity.getComponent<Transform>();
    auto p = entity.getComponent<RigidBody>();
    p->setTransform(*t);
    p->createRigidBody();

    p->addToWorld(mWorld);
  }
  void PhysicsSystem::entityRemoved(engine::Engine& engine, Entity& entity)
  {
    auto p = entity.getComponent<RigidBody>();
    p->removeFromWorld(mWorld);
  }

  void PhysicsSystem::run(engine::Engine& engine)
  {
    auto seconds = engine.getElapsed().asSeconds();
    mWorld->stepSimulation(seconds, 10);

    for( auto &e : mEntities )
    {
      auto entity = e.get();
      auto t = entity.getComponent<Transform>();

      auto p = entity.getComponent<RigidBody>();
      auto tr = p->getWorldTransform();
      t->set(*tr);
    }
  }
}
