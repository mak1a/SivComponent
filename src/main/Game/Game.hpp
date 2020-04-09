
#pragma once

//#include "../../ComponentEngine/ComponentEngine.hpp"
//#include "PhotonComponent.hpp"
//#include "SivComponent.hpp"

#include "Player/Player.hpp"
#include "Bullet.hpp"

class Game : public ComponentEngine::IScene
{
    void playermaker(std::shared_ptr<GameObject>& obj) {}

    void Setup() override;
    // メンバだと実体化してないと呼び出せない
    // staticにすると不要でも実装しないと呼び出せない
    // void EngineInit(ComponentEngine::SceneCommon& common) override
    // {
    //     common.AddObjectCreator("Player", [&]() {
    //         //        auto obj = GameObject::Create();
    //         auto obj = std::make_shared<GameObject>();
    //         obj->AddComponent<Player>();
    //         obj->AddComponent<Siv::Circle>()->SetShape({0, 0, 10});
    //         return obj;
    //     });
    // }
};
