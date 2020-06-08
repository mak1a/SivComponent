#pragma once

#include <list>

#include "../../ComponentEngine/AttachableComponent.hpp"
#include "../../UserDefinition/CollisionLayer.hpp"
#include "ICollider.hpp"
#include "ShapeTransform.hpp"

namespace ComponentEngine::Collision
{
    using LayerType = unsigned char;
    using Layer = UserDef::CollisionLayer;

    //衝突判定を取りたい物体に一つだけつける
    //各ColliderのStart呼び出し前につけないと機能しない
    class CollisionObject : public AttachableComponent
    {
    private:
        const LayerType layer;
        //判定の変更はほとんどないはずなので効率の良いvectorで管理する
        //ここスマポにするとshared_from_thisが必要になるので無理
        std::vector<ICollider*> colliders;

    public:
        CollisionObject(UserDef::CollisionLayer _layer = Layer::Default, size_t collidernum = 1)
            : layer(static_cast<LayerType>(_layer))
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

        LayerType GetLayer() const noexcept
        {
            return layer;
        }
    };

}  // namespace ComponentEngine::Collision
