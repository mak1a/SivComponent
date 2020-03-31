#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "Enemy.hpp"

class EnemyManager : public Photon::AttachableComponentPhotonCallbacks
{
    //敵生成に使うための値
    double borderValue;

    int enemynumber;

    void Start2() override;
    void Update() override;

    [[nodiscard]] int GenerateEnemyNumber();

public:
    void CreateStandardEnemy(const s3d::Vec2& position);
};