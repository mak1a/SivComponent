
#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "../../../Utilities/IntervalCall.hpp"

class Player;
class PlayerListView;

class PlayerManager : public ComponentEngine::Photon::AttachableComponentPhotonCallbacks
{
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;
    void leaveRoomEventAction(int playerNr, bool isInactive) override;
    void Start2() override;

    void Update() override;

    Utilities::IntervalCall initsync;

public:
    std::shared_ptr<GameObject> CreateMyPlayer();

    PlayerManager();
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<PlayerListView>> views;
};
