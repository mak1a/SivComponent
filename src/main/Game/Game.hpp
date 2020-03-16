
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
        //自分を作る
        auto mainplayer = CreateAndGetObject();
        mainplayer->AddComponent<Player>();
        players->AddChild(mainplayer);
    }
};
