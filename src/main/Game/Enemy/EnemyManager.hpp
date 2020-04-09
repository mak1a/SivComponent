#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "../GameMaster/Timer.hpp"
#include "Enemy.hpp"

enum EnemyType
{
    Standard = 5,
    Killer,
};

class EnemyManager : public Photon::AttachableComponentPhotonCallbacks
{
    //敵生成に使うための値
    struct
    {
        double interval = 0;
        double generatableHP = 20 * 3;
        double persecond = 15;
    } generator;

    void Start2() override;
    void Update() override;
    void customEventAction(int øplayerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

    std::shared_ptr<GameObject> bullets;

    void EnemyGenerate();

public:
    std::shared_ptr<GameObject> playercore;
    std::shared_ptr<Timer> timer;

    // void CreateStandardEnemy(const s3d::Vec2& position, bool isMine = false, int enemyNumber = -1);
    int CreateStandardEnemy(ExitGames::Common::Dictionary<nByte, int>* dic);

    void CreateBullet(Enemy& enemy, const s3d::Vec2& target, double spd, double lifetime, int attack = 10);
};