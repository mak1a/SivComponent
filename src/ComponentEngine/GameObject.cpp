
#include "GameObject.hpp"
#include "iostream"

#include <Siv3D.hpp>
#include "AttachableComponent.hpp"

namespace ComponentEngine
{
    std::weak_ptr<GameObject>& GameObject::GetParent()
    {
        return parent;
    }

    std::list<std::shared_ptr<GameObject>>& GameObject::GetChildren()
    {
        return children;
    }

    bool GameObject::IsInitializedAll()
    {
        return initializedAll;
    }

    s3d::Mat3x2& GameObject::parent_matrix() const
    {
        auto p = parent.lock();
        //親がなければ代わりの行列を返す
        if (!p)
        {
            return identity_matrix;
        }
        return p->_transform.matrix;
    }

    //子オブジェクトに変換行列を適用する
    void GameObject::update_children_matrix()
    {
        for (std::shared_ptr<GameObject>& child : children)
        {
            child->_transform.update_matrix(_transform.matrix);
            child->update_children_matrix();
        }
    }

    GameObject& GameObject::SetPosition(const s3d::Vec2& _position)
    {
        return SetLocalPosition(_position);
    }

    s3d::Vec2 GameObject::GetPosition() const
    {
        return GetLocalPosition();
    }

    GameObject& GameObject::SetWorldPosition(const s3d::Vec2& _position)
    {
        _transform.SetWorldPosition(_position, parent.lock()->transform().matrix.inversed(), parent_matrix());
        update_children_matrix();
        return *this;
    }

    s3d::Vec2 GameObject::GetWorldPosition() const
    {
        return _transform.GetWorldPosition();
    }

    GameObject& GameObject::SetLocalPosition(const s3d::Vec2& _position)
    {
        _transform.SetPosition(_position, parent_matrix());
        update_children_matrix();
        return *this;
    }

    s3d::Vec2 GameObject::GetLocalPosition() const
    {
        return _transform.GetPosition();
    }

    GameObject& GameObject::SetRotateByRadian(double _rotate)
    {
        _transform.SetRotateByRadian(_rotate, parent_matrix());
        update_children_matrix();
        return *this;
    }

    GameObject& GameObject::SetRotateByAngle(double angle)
    {
        _transform.SetRotateByAngle(angle, parent_matrix());
        update_children_matrix();
        return *this;
    }

    double GameObject::GetRotate() const noexcept
    {
        return _transform.GetRotate();
    }

    GameObject& GameObject::SetScale(double _scale)
    {
        _transform.SetScale(_scale, parent_matrix());
        update_children_matrix();
        return *this;
    }

    double GameObject::GetScale() const noexcept
    {
        return _transform.GetScale();
    }

    s3d::Vec2 GameObject::LookAt(const s3d::Vec2& targetWorldPos) const
    {
        auto thispos = this->GetLocalPosition();

        //相手の座標をこちらのローカル座標系に変換
        auto targetpos = parent.lock()->_transform.matrix.transform(targetWorldPos);

        //ベクトルを計算
        auto diff = targetpos - thispos;
        if (diff.isZero())
        {
            return diff;
        }
        diff.normalize();
        //正規化して返す
        return diff;
    }

    s3d::Vec2 GameObject::LookAt(const std::shared_ptr<GameObject>& target) const
    {
        return LookAt(target->GetWorldPosition());
    }

    GameObject& GameObject::SetName(const std::string& _name)
    {
        name = _name;
        return *this;
    }

    std::string GameObject::GetName() const noexcept
    {
        return name;
    }

    GameObject& GameObject::SetTag(UserDef::Tag _tag)
    {
        objecttag = _tag;
        return *this;
    }

    UserDef::Tag GameObject::GetTag() const
    {
        return objecttag;
    }

    std::weak_ptr<IScene> GameObject::GetScene()
    {
        // sceneの参照を持ってなかったら親から回収する
        if (scene.expired())
        {
            scene = parent.lock()->GetScene();
        }

        return scene;
    }

    GameObject& GameObject::SetActive(bool _active)
    {
        active = _active;
        // HACK: 子オブジェクトも連動して変化するようにしているが、このあたりは要検討。
        for (const auto& c : children)
        {
            c->SetActive(_active);
        }
        return *this;
    }

    bool GameObject::GetActive() const noexcept
    {
        return active;
    }

    //プロパティ
    Transform& GameObject::transform()
    {
        return _transform;
    }

    GameObject::GameObject()
        : GameObject(Transform())
    {
    }

    //実際に動くコンストラクタ
    GameObject::GameObject(const Transform& trans)
    {
        active = true;
        _transform = trans;
        objecttag = UserDef::Tag::Default;
    }

    //<return>child
    const std::shared_ptr<GameObject>& GameObject::AddChild(const std::shared_ptr<GameObject>& child, bool pushfront)
    {
        child->SetParent(weak_from_this(), pushfront);
        return child;
    }

    std::shared_ptr<GameObject> GameObject::CreateChild()
    {
        return CreateChild("");
    }

    std::shared_ptr<GameObject> GameObject::CreateChild(const std::string& objectName)
    {
        auto object = std::make_shared<GameObject>();
        object->SetName(objectName);
        object->scene = this->scene;
        this->AddChild(object);
        return object;
    }

    void GameObject::SetParent(const std::weak_ptr<GameObject>& newParent, bool pushfront)
    {
        //現在の関係をデバッグ出力
        // std::cout << "this is:" << GetName() << std::endl;

        if (!newParent.lock())
        {
            return;
        }

        //自分を新しい親の子オブジェクトに設定
        if (pushfront)
        {
            newParent.lock()->children.push_front(shared_from_this());
        }
        else

        {
            newParent.lock()->children.push_back(shared_from_this());
        }

        if (parent.lock())
        {
            //今の親と関係を解消
            parent.lock()->RemoveChild(shared_from_this());
        }

        //親を新しいオブジェクトに設定
        parent = newParent;
        //親の変換座標を使って自分の変換座標を更新
        this->_transform.update_matrix(parent.lock()->_transform.matrix);
    }

    auto GameObject::FindChildItr(const std::shared_ptr<GameObject>& child) const -> decltype(std::find(children.begin(), children.end(), child))
    {
        return std::find(children.begin(), children.end(), child);
    }

    auto GameObject::FindChildItr(const std::string& name) const -> decltype(std::find(children.begin(), children.end(), children.front()))
    {
        return std::find_if(children.begin(), children.end(), [&](const std::shared_ptr<GameObject>& obj) { return obj->GetName() == name; });
    }

    std::shared_ptr<GameObject> GameObject::FindChild(const std::shared_ptr<GameObject>& child) const
    {
        auto itr = FindChildItr(child);

        if (itr == children.end())
        {
            return std::shared_ptr<GameObject>();
        }

        return *itr;
    }

    std::shared_ptr<GameObject> GameObject::FindChild(const std::string& name) const
    {
        auto itr = std::find_if(children.begin(), children.end(), [&](const std::shared_ptr<GameObject>& obj) { return obj->GetName() == name; });

        if (itr == children.end())
        {
            return std::shared_ptr<GameObject>();
        }

        return *itr;
    }

    std::shared_ptr<GameObject> GameObject::FindChildByName(const std::string& name) const
    {
        for (auto& child : children)
        {
            if (child->name == name)
            {
                return child;
            }
        }

        for (auto& child : children)
        {
            auto x = child->FindChildByName(name);

            if (x)
            {
                return x;
            }
        }

        return nullptr;
    }

    void GameObject::DeleteChild(const std::shared_ptr<GameObject>& child)
    {
        auto itr = FindChildItr(child);
        if (itr == children.end())
        {
            return;
        }

        child->DestroyAllChildrenComponents();

        children.erase(itr);
    }

    //子オブジェクトを削除せずに外します
    bool GameObject::RemoveChild(const std::shared_ptr<GameObject>& child)
    {
        auto itr = FindChildItr(child);

        if (itr == children.end())
        {
            return false;
        }

        children.erase(itr);
        return true;
    }

    //デストラクタに任せると伝播方向の関係でポインタが無効になって死ぬので先に呼べ
    //すべての子オブジェクトを破壊する
    void GameObject::DestroyAll()
    {
        if (GetName() == "MasterObject")
        {
            auto itr = FindChildItr("CommonParent");
            if (itr != children.end())
            {
                children.erase(itr);
            }
        }

        //まずはすべてのコンポーネントのOnDestoryを呼び出す
        DestroyAllChildrenComponents();
        //次にすべての子オブジェクトを削除
        DestroyAllChildren();
    }

    void GameObject::DestroyAllChildren()
    {
        for (const auto& child : children)
        {
            child->DestroyAllChildrenComponents();
        }

        children.clear();
    }

    void GameObject::DestroyAllChildrenComponents()
    {
        for (const auto& component : components)
        {
            component->OnDestroy();
        }
        for (const auto& child : children)
        {
            child->DestroyAllChildrenComponents();
        }
    }

    void GameObject::components_start()
    {
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

        for (const auto& child : children)
        {
            if (!child->GetActive())
            {
                continue;
            }
            // TODO: Transformの処理
            child->components_start();
        }
    }

    void GameObject::components_update()
    {
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
            child->components_update();
        }
    }

    void GameObject::components_lateUpdate()
    {
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
            child->components_lateUpdate();
        }
    }

    void GameObject::components_draw() const
    {
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
            child->components_draw();
        }
    }

    void GameObject::components_call_collisionstay(std::shared_ptr<GameObject>& object)
    {
        for (auto& c : components)
        {
            c->OnStayCollision(object);
        }
    }

    s3d::Mat3x2 GameObject::identity_matrix = s3d::Mat3x2::Identity();
}  // namespace ComponentEngine
