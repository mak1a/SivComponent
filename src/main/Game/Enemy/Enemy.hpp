
#pragma once

#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "./../../../ComponentEngine/ComponentEngine.hpp"

class Enemy : public Photon::AttachableComponentPhotonCallbacks
{
    int life = 30;
    int enemynumber;

    void OnStayCollision(std::shared_ptr<GameObject>& obj) override;

public:
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
