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
}  // namespace ComponentEngine::UserDef
