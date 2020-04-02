#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class EnemyManager;
class UIManager;

class Timer : public AttachableComponent
{
public:
    const int GAMETIME = 30;
    std::shared_ptr<Siv::Text> text;
    s3d::Stopwatch watch;

    std::shared_ptr<EnemyManager> enemyManager;
    std::shared_ptr<UIManager> uimanager;

private:
    void Start() override;
    void Update() override;
};

//タイマー起動用
class TimerSetup : public Photon::AttachableComponentPhotonCallbacks
{
    void Start2() override;
    void Update() override;

public:
    std::shared_ptr<Timer> timerobject;
    std::shared_ptr<Siv::Text> text;
};