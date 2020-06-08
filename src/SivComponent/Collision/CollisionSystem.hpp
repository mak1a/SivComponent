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

        std::vector<std::pair<LayerType, LayerType>> collisionTable;

        std::array<Layers, UserDef::COLLISIONLAYER_SIZE> colliders;

    public:
        CollisionSystem()
        {
            //エンジン内に直書きはちょっと…

            //デフォルト
            collisionTable.push_back(std::make_pair((LayerType)UserDef::CollisionLayer::Default, (LayerType)UserDef::CollisionLayer::Default));

            //フィールドとの判定
            collisionTable.push_back(std::make_pair((LayerType)UserDef::CollisionLayer::Field, (LayerType)UserDef::CollisionLayer::Player));
            collisionTable.push_back(std::make_pair((LayerType)UserDef::CollisionLayer::Field, (LayerType)UserDef::CollisionLayer::Enemy));
            collisionTable.push_back(std::make_pair((LayerType)UserDef::CollisionLayer::Field, (LayerType)UserDef::CollisionLayer::PlayerBullet));
            collisionTable.push_back(std::make_pair((LayerType)UserDef::CollisionLayer::Field, (LayerType)UserDef::CollisionLayer::EnemyBullet));

            //弾の判定
            collisionTable.push_back(std::make_pair((LayerType)UserDef::CollisionLayer::PlayerBullet, (LayerType)UserDef::CollisionLayer::Enemy));
            collisionTable.push_back(std::make_pair((LayerType)UserDef::CollisionLayer::EnemyBullet, (LayerType)UserDef::CollisionLayer::Player));

            //敵とプレイヤーの接触
            collisionTable.push_back(std::make_pair((LayerType)UserDef::CollisionLayer::Player, (LayerType)UserDef::CollisionLayer::Enemy));
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
                    return;
                }
            }
        }

        void CollisionCall()
        {
            // std::vector<std::pair<Layer, Layer>> table;
            // // PlayerとEnemyの仮判定
            // table.push_back(std::make_pair((Layer)UserDef::CollisionLayer::Player, (Layer)UserDef::CollisionLayer::Enemy));
            // collisionTable = table;
            for (const auto& pair : collisionTable)
            {
                //同レイヤーだった場合は自分同士の判定を取らないようにする必要がある
                // if (pair.first == pair.second)
                // {
                //     CollitionCallSameLayer(pair.first);
                //     continue;
                // }

                auto& colsa = colliders[pair.first];
                auto& colsb = colliders[pair.second];

                for (CollisionObject* a : colsa)
                {
                    // オブジェクトとコンポーネント両者とも動いてなければスルー
                    if (!(a->GetActive() && a->GetGameObject().lock()->GetActive()))
                    {
                        continue;
                    }

                    for (CollisionObject* b : colsb)
                    {
                        if (!(b->GetActive() && b->GetGameObject().lock()->GetActive()))
                        {
                            continue;
                        }

                        //同じレイヤー同士だと同じオブジェクトが来ることがあるのでスルー
                        if (a == b)
                        {
                            continue;
                        }

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

        // private:
        // void CollitionCallSameLayer(ComponentEngine::Collision::LayerType layer)
        // {
        //     auto& colsa = colliders[layer];

        //     for (CollisionObject* a : colsa)
        //     {
        //         // オブジェクトとコンポーネント両者とも動いてなければスルー
        //         if (!(a->GetActive() && a->GetGameObject().lock()->GetActive()))
        //         {
        //             continue;
        //         }

        //         for (CollisionObject* b : colsb)
        //         {
        //             if (!(b->GetActive() && b->GetGameObject().lock()->GetActive()))
        //             {
        //                 continue;
        //             }

        //             // GameObject同士の判定
        //             if (b->intersects(a))
        //             {
        //                 //それぞれのGameObjectのAPIコール
        //                 auto obja = a->GetGameObject().lock();
        //                 auto objb = b->GetGameObject().lock();

        //                 obja->components_call_collisionstay(objb);

        //                 objb->components_call_collisionstay(obja);
        //             }
        //         }
        //     }
        // }
    };

}  // namespace ComponentEngine::Collision
