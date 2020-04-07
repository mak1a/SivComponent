#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class EnemyManager;
class UIManager;

class Timer : public Photon::AttachableComponentPhotonCallbacks
{
public:
    const int GAMETIME = 100;

    std::shared_ptr<EnemyManager> enemyManager;
    std::shared_ptr<UIManager> uimanager;

    int GetTime() const
    {
        return GAMETIME - watch.s();
    }

private:
    std::shared_ptr<Siv::Text> text;
    s3d::Stopwatch watch;

    void Start2() override;
    void Update() override;
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;
};

//タイマー起動用
class TimerSetup : public Photon::AttachableComponentPhotonCallbacks
{
    void Start2() override;
    void Update() override;

public:
    std::shared_ptr<Timer> timerobject;
    std::shared_ptr<Siv::Text> text;
    std::shared_ptr<GameObject> explanation;
};