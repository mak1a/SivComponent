#pragma once

#include <list>

#include "../../ComponentEngine/AttachableComponent.hpp"
#include "../../UserDefinition/CollisionLayer.hpp"
#include "ICollider.hpp"
#include "ShapeTransform.hpp"

namespace ComponentEngine::Collision
{
    using Layer = unsigned char;

    //衝突判定を取りたい物体に一つだけつける
    //各ColliderのStart呼び出し前につけないと機能しない
    class CollisionObject : public AttachableComponent
    {
    private:
        const Layer layer;
        //判定の変更はほとんどないはずなのでvectorで管理する
        // shared_from_thisを噛ませないといけなくなるので生ポインタ管理　リソース適切に管理すれば問題なし
        std::vector<ICollider*> colliders;

    public:
        CollisionObject(UserDef::CollisionLayer _layer, size_t collidernum = 1)
            : layer(static_cast<Layer>(_layer))

        {
            colliders.reserve(collidernum);
        }

        void Start() override;
        void OnDestroy() override;

        void Register(ICollider* col)
        {
            colliders.push_back(col);
        }

        void Dispose(ICollider* col)
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

        //衝突判定を取得したい物体同士の衝突
        bool intersects(CollisionObject* collision)
        {
            auto& cols = collision->colliders;
            for (ICollider* c : cols)
            {
                for (ICollider* cc : colliders)
                {
                    //お互いのもつ判定の総当たり
                    if (c->intersects(cc))
                    {
                        //ひとつでも衝突していたら完了
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
