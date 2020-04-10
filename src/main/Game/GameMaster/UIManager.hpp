#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class UIManager : public Photon::AttachableComponentPhotonCallbacks
{
    void disconnectReturn() override
    {
        objects.playerManager->SetActive(false);
        objects.playerBulletManager->SetActive(false);
        objects.timeUI->SetActive(false);
        objects.enemyManager->SetActive(false);

        objects.connectError->SetActive(true);
        objects.returnTitleBt->SetActive(true);
    }

public:
    void OnVictory();
    void OnDefeat();

    //インテリセンス用
    struct
    {
        std::shared_ptr<GameObject> playerManager, playerBulletManager, enemyManager, victoryUI, defeatUI, timeUI, returnTitleBt, connectError;
    } objects;
};
