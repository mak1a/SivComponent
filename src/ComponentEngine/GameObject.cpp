
#include "GameObject.hpp"
#include "iostream"

#include <Siv3D.hpp>
#include "AttachableComponent.hpp"

namespace ComponentEngine
{
    void GameObject::components_start()
    {
        for (IComponent* component : components)
        {
            component->call_start();
        }
        initializedAll = true;

        for (const auto& child : children)
        {
            // TODO: Transformの処理
            child->components_start();
        }
    }

    void GameObject::components_update()
    {
        //変換行列を作成
        auto trans = _transform.PushTransform();

        for (IComponent* component : components)
        {
            component->Update();
        }

        for (const auto& child : children)
        {
            child->components_update();
        }
    }

    void GameObject::components_lateUpdate()
    {
        for (IComponent* component : components)
        {
            component->LateUpdate();
        }

        for (const auto& child : children)
        {
            child->components_lateUpdate();
        }
    }

    void GameObject::components_draw() const
    {
        //変換行列を作成
        auto trans = _transform.PushTransform();

        // z情報を設定
        // s3d::Graphics2D::SetZ();
        //あっ
        //デプスステート機能がまだ移植されてなかった…

        for (IComponent* component : components)
        {
            component->Draw();
        }

        for (const auto& child : children)
        {
            child->components_draw();
        }
    }
}  // namespace ComponentEngine
