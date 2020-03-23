
#include "GameObject.hpp"
#include "iostream"

#include <Siv3D.hpp>
#include "AttachableComponent.hpp"

namespace ComponentEngine
{
    void GameObject::components_start(const s3d::Mat3x2& _matrix)
    {
        _transform.update_matrix(_matrix);

        if (!initializedAll)
        {
            //全部初期化後は呼び出しが無駄なので処理カット用
            initializedAll = true;

            for (auto& component : components)
            {
                if (!component->GetActive())
                {
                    //ひとつでも初期化できていなかったら次もチェックを行う
                    initializedAll = false;
                    continue;
                }

                component->call_start();
            }
        }

        //子オブジェクトも初期化済みだったら飛ばすようにしたい
        for (const auto& child : children)
        {
            if (!child->GetActive())
            {
                continue;
            }
            // TODO: Transformの処理
            child->components_start(_transform.matrix);
        }
    }

    void GameObject::components_update(const s3d::Mat3x2& _matrix)
    {
        _transform.update_matrix(_matrix);

        for (auto& component : components)
        {
            if (!component->GetActive() || !component->_initialized())
            {
                continue;
            }
            component->Update();
        }

        for (const auto& child : children)
        {
            if (!child->GetActive())
            {
                continue;
            }
            child->components_update(_transform.matrix);
        }
    }

    void GameObject::components_lateUpdate(const s3d::Mat3x2& _matrix)
    {
        _transform.update_matrix(_matrix);

        for (auto& component : components)
        {
            if (!component->GetActive() || !component->_initialized())
            {
                continue;
            }
            component->LateUpdate();
        }

        for (const auto& child : children)
        {
            if (!child->GetActive())
            {
                continue;
            }
            child->components_lateUpdate(_transform.matrix);
        }
    }

    void GameObject::components_draw(const s3d::Mat3x2& _matrix) const
    {
        _transform.update_matrix(_matrix);

        // z情報を設定
        // s3d::Graphics2D::SetZ();
        //あっ
        //デプスステート機能がまだ移植されてなかった…

        for (const auto& component : components)
        {
            if (!component->GetActive() || !component->_initialized())
            {
                continue;
            }
            component->Draw();
        }

        for (const auto& child : children)
        {
            if (!child->GetActive())
            {
                continue;
            }
            child->components_draw(_transform.matrix);
        }
    }

    void GameObject::components_call_collisionstay(std::shared_ptr<GameObject>& object)
    {
        for (auto& c : components)
        {
            c->OnStayCollision(object);
        }
    }
}  // namespace ComponentEngine
