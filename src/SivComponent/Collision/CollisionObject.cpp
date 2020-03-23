
#include "CollisionObject.hpp"
#include "../../ComponentEngine/GameObject.hpp"
#include "../../ComponentEngine/IScene.hpp"

namespace ComponentEngine::Collision
{
    void CollisionObject::Start()
    {
        auto x = GetGameObject().lock()->GetScene().lock();
        auto& y = x->GetCollisionSystem();
        y.Register(this);
    }
    void CollisionObject::OnDestroy()
    {
        auto x = GetGameObject().lock()->GetScene().lock();
        auto& y = x->GetCollisionSystem();
        y.Dispose(this);
    }
}  // namespace ComponentEngine::Collision
