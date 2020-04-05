#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "Enemy.hpp"

enum EnemyType
{
    Standard = 5,
    Killer,
};

class EnemyManager : public Photon::AttachableComponentPhotonCallbacks
{
    //敵生成に使うための値
    double generateTime = 0;

    void Start2() override;
    void Update() override;
    void customEventAction(int øplayerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

    std::shared_ptr<GameObject> bullets;

public:
    std::shared_ptr<GameObject> playercore;

    // void CreateStandardEnemy(const s3d::Vec2& position, bool isMine = false, int enemyNumber = -1);
    void CreateStandardEnemy(ExitGames::Common::Dictionary<nByte, int>* dic);

    void CreateBullet(Enemy& enemy, const s3d::Vec2& target, double spd, double lifetime, int attack = 10);
};