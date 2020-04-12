#pragma once

namespace ComponentEngine::UserDef
{
    //数値はデフォルトのままにしておくこと
    enum class CollisionLayer
    {
        Player,
        Enemy,
        PlayerBullet,
        EnemyBullet,
        Field,
        COLLISIONLAYER_SIZE,
    };
constexpr    size_t COLLISIONLAYER_SIZE = static_cast<size_t>(CollisionLayer::COLLISIONLAYER_SIZE);
}  // namespace ComponentEngine::UserDef
