#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class UIManager : public Photon::AttachableComponentPhotonCallbacks
{
    void OnVictory();
    void OnDefeat();
    //通信接続が切れた時の処理

public:
    //インテリセンス用
    struct
    {
        std::shared_ptr<GameObject> playerManager, enemyManager, victoryUI, defeatUI, timeUI, returnTitleBt;
    } objects;
};
