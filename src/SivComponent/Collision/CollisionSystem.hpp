#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>

#include <boost/noncopyable.hpp>
#include <list>

#include "../../ComponentEngine/GameObject.hpp"
#include "../../UserDefinition/CollisionLayer.hpp"
#include "CollisionObject.hpp"
#include "ICollider.hpp"

namespace ComponentEngine::Collision
{
    class CollisionSystem : boost::noncopyable
    {
    private:
        using Layers = std::list<CollisionObject*>;

        std::vector<std::pair<Layer, Layer>> collisionTable;

        std::array<Layers, UserDef::COLLISIONLAYER_SIZE> colliders;

    public:
        
        CollisionSystem()
        {
            
        }
        
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
            // PlayerとEnemyの仮判定
            table.push_back(std::make_pair((Layer)UserDef::CollisionLayer::Player, (Layer)UserDef::CollisionLayer::Enemy));
            collisionTable = table;
            for (const auto& pair : collisionTable)
            {
                auto& colsa = colliders[pair.first];
                auto& colsb = colliders[pair.second];

                for (CollisionObject* a : colsa)
                {
                    for (CollisionObject* b : colsb)
                    {
                        // GameObject同士の判定
                        if (b->intersects(a))
                        {
                            //それぞれのGameObjectのAPIコール
                            auto obja = a->GetGameObject().lock();
                            auto objb = b->GetGameObject().lock();

                            obja->components_call_collisionstay(objb);

                            objb->components_call_collisionstay(obja);
                        }
                    }
                }
            }
        }
    };

}  // namespace ComponentEngine::Collision
