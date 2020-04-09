#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class UIManager : public Photon::AttachableComponentPhotonCallbacks
{
public:
    void OnVictory();
    void OnDefeat();

    //インテリセンス用
    struct
    {
        std::shared_ptr<GameObject> playerManager, playerBulletManager, enemyManager, victoryUI, defeatUI, timeUI, returnTitleBt;
    } objects;
};
