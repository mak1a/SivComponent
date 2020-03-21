#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>

#include <list>

#include "../../UserDefinition/CollisionLayer.hpp"
#include "ColliderBase.hpp"
#include "CollisionObject.hpp"

namespace ComponentEngine::Collision
{
    //    class CollisionObject;

    class CollisionSystem
    {
    private:
        using Layers = std::list<CollisionObject*>;

        std::vector<std::pair<Layer, Layer>> collisionTable;

        std::array<Layers, UserDef::COLLISIONLAYER_SIZE> colliders;

    public:
        void Register(CollisionObject* collider)
        {
            int layer = collider->GetLayer();

            colliders[layer].push_back(collider);
        }

        void Dispose(CollisionObject* collider)
        {
            int layer = collider->GetLayer();

            auto& col = colliders[layer];

            const auto end = col.end();
            for (auto itr = col.begin(); itr != end; ++itr)
            {
                if (*itr == collider)
                {
                    col.erase(itr);
                    std::cout << "disposed" << std::endl;
                    return;
                }
            }
        }

        void CollisionCall()
        {
            std::vector<std::pair<Layer, Layer>> table;
            table.push_back(std::make_pair((Layer)UserDef::CollisionLayer::Player, (Layer)UserDef::CollisionLayer::Enemy));

            for (const auto& pair : collisionTable)
            {
                auto& colsa = colliders[pair.first];
                auto& colsb = colliders[pair.second];

                for (CollisionObject* a : colsa)
                {
                    for (CollisionObject* b : colsb)
                    {
                        if (a->intersects(b))
                        {
                        }
                    }
                }
            }
        }
    };

}  // namespace ComponentEngine::Collision
