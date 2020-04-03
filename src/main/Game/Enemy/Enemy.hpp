
#pragma once

#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "./../../../ComponentEngine/ComponentEngine.hpp"

class EnemyManager;
class PlayerManager;
class PlayerCore;

class Enemy : public Photon::AttachableComponentPhotonCallbacks
{
    int life = 30;
    int spd = 35;
    //敵ごとの固有ID
    int enemynumber;
    bool isMine = false;

    struct
    {
        double interval = 0;     //連射間隔を空ける
        int rapidnum = 0;        //連射回数
        double reloadtime = -1;  //次の射撃開始までの時間
        std::shared_ptr<GameObject> targetplayer;
    } fire;

    void OnStayCollision(std::shared_ptr<GameObject>& obj) override;
    void Start2() override;
    void Update() override;
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

    std::shared_ptr<GameObject> NearestObject();
    void SetTarget();

    void Move();
    void Shot();
    void Damage(int damage);
    void SendSyncInfo(int damage, int targetPlayer = -99) const;

public:
    static EnemyManager* enemyManager;
    static PlayerManager* playerManager;
    std::shared_ptr<GameObject> TargetObject;

    void SetEnemyNumber(int n)
    {
        enemynumber = n;
    }

    void SetIsMine(bool ismine)
    {
        isMine = ismine;
    }

    bool GetIsMine() const
    {
        return isMine;
    }

    int GetEnemyNumber() const
    {
        return enemynumber;
    }

    int GetLife() const
    {
        return life;
    }
};
