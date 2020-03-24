
#pragma once

//#include "../../ComponentEngine/ComponentEngine.hpp"
//#include "PhotonComponent.hpp"
//#include "SivComponent.hpp"

#include "Player/Player.hpp"
#include "Player/PlayerBullet.hpp"

class Game : public ComponentEngine::IScene
{
    void playermaker(std::shared_ptr<GameObject>& obj) {}

    void Setup() override
    {
        s3d::Scene::SetBackground(s3d::Palette::Lightgrey);

        auto players = CreateAndGetObject();
        players->SetName("Players");

        //自分を作る
        auto mainplayerobj = GetSceneManager()->GetCommon().Instantiate("Player");
        auto system = GetSceneManager()->GetCommon().GetCommonObject(Photon::NetworkObjectName())->GetComponent<Photon::NetworkSystem>();

        mainplayerobj->SetPosition(Player::playerInitpos[system->GetPlayerNumberInRoom()]);

        auto player = mainplayerobj->GetComponent<Player>();
        player->SetMine(true);
        player->playerNr = system->GetClient().getLocalPlayer().getNumber();

        players->AddComponent<PlayerMaster>()->players.push_back(player);
        //    player->SendInstantiateMessage();

        players->AddChild(mainplayerobj);

        auto o = CreateAndGetObject();
        o->AddComponent<BulletManager>()->player = player;
    }

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
