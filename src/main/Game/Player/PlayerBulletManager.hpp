#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "../Bullet.hpp"

class Player;

class PlayerBulletManager : public Photon::AttachableComponentPhotonCallbacks
{
    ComponentEngine::SceneCommon::Functype inst;
    double fireInterval;

    void Start2() override;
    void Update() override;

    void CreateBullet();
    void SendBulletInfo(std::shared_ptr<Bullet>& bullet);
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

public:
    std::shared_ptr<Player> player;
};
