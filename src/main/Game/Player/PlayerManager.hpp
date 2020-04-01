
#pragma once

#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "../../../Utilities/IntervalCall.hpp"

class Player;

class PlayerManager : public ComponentEngine::Photon::AttachableComponentPhotonCallbacks
{
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;
    void leaveRoomEventAction(int playerNr, bool isInactive) override;
    void Start2() override;

    void Update() override;

    Utilities::IntervalCall initsync;

public:
    PlayerManager();
    std::vector<std::shared_ptr<Player>> players;
};
