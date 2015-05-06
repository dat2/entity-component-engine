// libraries

// engine
#include <components/transform.hpp>
#include <components/camera.hpp>
#include <engine/engine.hpp>
#include <systems/physics/physics.hpp>
#include <utils/utils.hpp>

using namespace utils;

namespace systems
{
  PhysicsSystem::PhysicsSystem()
    : System("PhysicsSystem", TRANSFORM),
    mBroadphase(nullptr), mCollisionConfiguration(nullptr), mDispatcher(nullptr),
    mSolver(nullptr), mWorld(nullptr),

    mGroundShape(nullptr), mGroundRigidBody(nullptr),
    mFallShape(nullptr), mFallRigidBody(nullptr)
  {
    createWorld();
    fillWorld();
  }

  PhysicsSystem::~PhysicsSystem()
  {
    mWorld->removeRigidBody(mFallRigidBody);
    delete mFallRigidBody->getMotionState();
    delete mFallRigidBody;

    delete mFallShape;

    mWorld->removeRigidBody(mGroundRigidBody);
    delete mGroundRigidBody->getMotionState();
    delete mGroundRigidBody;

    delete mGroundShape;

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

  void PhysicsSystem::fillWorld()
  {
    // create ground plane
    mGroundShape = new btStaticPlaneShape(btVector3(0, 1, 0), -6);

    btDefaultMotionState* mGroundMotionState =
      new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

    btRigidBody::btRigidBodyConstructionInfo
      mGroundRigidBodyCI(0, mGroundMotionState, mGroundShape, btVector3(0, 0, 0));
    mGroundRigidBody = new btRigidBody(mGroundRigidBodyCI);

    mWorld->addRigidBody(mGroundRigidBody);

    // create falling ball
    mFallShape = new btSphereShape(1);

    btDefaultMotionState* mFallMotionState =
      new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    mFallShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo mFallRigidBodyCI(mass, mFallMotionState, mFallShape, fallInertia);
    mFallRigidBody = new btRigidBody(mFallRigidBodyCI);

    mWorld->addRigidBody(mFallRigidBody);
  }

  void PhysicsSystem::run(engine::Engine& engine)
  {
    auto seconds = engine.getElapsed().asSeconds();
    mWorld->stepSimulation(seconds, 10);

    btTransform trans;
    mFallRigidBody->getMotionState()->getWorldTransform(trans);

    auto e = engine.getEntity("tree");
    auto entity = *e;
    auto t = entity.getComponent<Transform>();
    t->set(trans);

    // Camera follow the transform
    if(trans.getOrigin().getY() > -5)
    {
      auto p = engine.getEntity("player");
      auto player = *p;
      player.getComponent<Camera>()->lookAt(bulletToSfml(trans.getOrigin()));
    }
  }
}
