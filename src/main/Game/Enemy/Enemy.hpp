
#pragma once

#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "./../../../ComponentEngine/ComponentEngine.hpp"

class EnemyManager;

class Enemy : public Photon::AttachableComponentPhotonCallbacks
{
    int life = 30;
    double fireInterval = 0;
    int enemynumber;

    void OnStayCollision(std::shared_ptr<GameObject>& obj) override;

    // void Start2() override;
    void Update() override;

public:
    EnemyManager* enemyManager;

    void SetEnemyNumber(int n)
    {
        enemynumber = n;
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
