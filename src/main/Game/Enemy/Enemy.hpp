
#pragma once

#include "./../../../ComponentEngine/ComponentEngine.hpp"

using namespace ComponentEngine;

class Enemy : public AttachableComponent
{
    void OnStayCollision(std::shared_ptr<GameObject>& obj) override
    {
        //弾当たったら
        if (obj->GetTag() == UserDef::Tag::PlayerBullet)
        {
            GetGameObject().lock()->GetScene().lock()->Destroy(GetGameObject().lock());
        }
    }
};
