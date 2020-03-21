#pragma once

#include <list>

#include "../../ComponentEngine/AttachableComponent.hpp"
#include "../../UserDefinition/CollisionLayer.hpp"
#include "ColliderBase.hpp"

namespace ComponentEngine::Collision
{
    using Layer = unsigned char;

    class ColliderBase;

    //衝突判定を取りたい物体に一つだけつける
    //各ColliderのStart呼び出し前につけないと機能しない
    class CollisionObject : public AttachableComponent
    {
    private:
        const Layer layer;
        //判定の変更はほとんどないはずなのでvectorで管理する
        // shared_from_thisを噛ませないといけなくなるので生ポインタ管理　リソース適切に管理すれば問題なし
        std::vector<ColliderBase*> colliders;

    public:
        CollisionObject(UserDef::CollisionLayer _layer)
            : layer(static_cast<Layer>(_layer))
            , colliders(1)
        {
        }

        void Start() override;

        void Register(ColliderBase* col)
        {
            colliders.push_back(col);
        }

        void Dispose(ColliderBase* col)
        {
            const auto end = colliders.end();

            for (auto itr = colliders.begin(); itr != end; ++itr)
            {
                if (col == *itr)
                {
                    colliders.erase(itr);
                    return;
                }
            }
        }

        bool intersects(CollisionObject* collision)
        {
            auto& cols = collision->colliders;
            for (ColliderBase* c : cols)
            {
                for (ColliderBase* cc : colliders)
                {
                    if (c->intersects(cc->shape))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        Layer GetLayer() const noexcept
        {
            return layer;
        }
    };

}  // namespace ComponentEngine::Collision
