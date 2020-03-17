
#pragma once

#include "ComponentEngine.hpp"
#include "PhotonComponent.hpp"
#include "SivComponent.hpp"

#include "Player.hpp"

class Game : public ComponentEngine::IScene
{
    void playermaker(std::shared_ptr<GameObject>& obj) {}

    void Setup() override
    {
        auto players = CreateAndGetObject();
        players->SetName("Players");
        players->AddComponent<PlayerCreator>();

        //自分を作る
        auto mainplayerobj = GetSceneManager()->GetCommon().Instantiate("Player");
        auto system = GetSceneManager()->GetCommon().GetCommonObject(Photon::NetworkObjectName())->GetComponent<Photon::NetworkSystem>();

        mainplayerobj->transform().SetPosition(Player::playerInitpos[system->GetPlayerNumberInRoom()]);

        auto player = mainplayerobj->GetComponent<Player>();
        player->SetMine(true);
        player->playerNr = system->GetClient().getLocalPlayer().getNumber();
//        player->SendInstantiateMessage();

        players->AddChild(mainplayerobj);
    }
};
