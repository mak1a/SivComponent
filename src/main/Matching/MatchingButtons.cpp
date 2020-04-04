
#include "MatchingButtons.hpp"

#include "../../Utilities/PhotonUtilities.hpp"
#include "../CustomEventList.hpp"
#include "Matching.hpp"

void PlayerListDisplay::Start2()
{
    text = GetGameObject().lock()->GetComponent<Siv::Text>();
}

void PlayerListDisplay::UpdateDisplay()
{
    auto plist = networkSystem->GetPlayerList();

    s3d::String str;
    for (int i = 0; i < plist.size(); ++i)
    {
        int num = Utilities::GetPlayerNumber(plist[i]->getName());

        const s3d::String you = ((plist[i]->getNumber() == networkSystem->GetClient().getLocalPlayer().getNumber()) ? U" : you" : U"");

        str += s3d::Format(i + 1, U" : Player", num, you, U"\n");
    }
    text->SetText(str);
}

void PlayerListDisplay::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
    UpdateDisplay();
};

void PlayerListDisplay::leaveRoomEventAction(int playerNr, bool isInactive)
{
    UpdateDisplay();
}

//=========================

void MatchSystem::Start2()
{
    auto object = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetCommonObject("PhotonSystem");

    networkSystem->Connect();
    s3d::Print(U"connecting...");

    networkSystem->GetClient().fetchServerTimestamp();
}

void MatchSystem::connectReturn(int errorCode,
                                const ExitGames::Common::JString& errorString,
                                const ExitGames::Common::JString& region,
                                const ExitGames::Common::JString& cluster)
{
    if (errorCode)
    {
        s3d::Print(U"connect error.");

        GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
        return;
    }
    s3d::Print(U"connected!");

    networkSystem->GetClient().opJoinRandomRoom(ExitGames::Common::Hashtable(), 4);

    s3d::Print(U"Search Room...");
};

void MatchSystem::joinRandomRoomReturn(int localPlayerNr,
                                       const ExitGames::Common::Hashtable& roomProperties,
                                       const ExitGames::Common::Hashtable& playerProperties,
                                       int errorCode,
                                       const ExitGames::Common::JString& errorString)

{
    if (errorCode)
    {
        s3d::Print(U"Room not found.");

        networkSystem->GetClient().opCreateRoom(L"", ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(4));

        s3d::Print(U"Create Room...");
    }
    s3d::Print(U"Connected Room!");
}

void MatchSystem::joinRoomReturn(int localPlayerNr,
                                 const ExitGames::Common::Hashtable& roomProperties,
                                 const ExitGames::Common::Hashtable& playerProperties,
                                 int errorCode,
                                 const ExitGames::Common::JString& errorString)
{
    if (errorCode)
    {
        s3d::Print(U"CreateRoom error!");
        s3d::Print(U"Please back to Title.");

        GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
        return;
    }

    s3d::Print(U"Join Room!");
}

// void disconnectReturn()
// {
//     s3d::Print(U"disconnectReturn");
// }

// void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
// {
//     s3d::Print(U"leaveRoomReturn");
// }

void MatchSystem::OnDestory2()
{
    s3d::ClearPrint();
}

void MatchSystem::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::MatchingSync)
    {
        return;
    }

    //情報取得
    ExitGames::Common::Hashtable table = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent).getDataCopy();

    Matching::Difficulty = ExitGames::Common::ValueObject<int>(table.getValue(static_cast<short>(0))).getDataCopy();

    Matching::GameStartTime = ExitGames::Common::ValueObject<int>(table.getValue(static_cast<short>(1))).getDataCopy();

    bool gameStart = ExitGames::Common::ValueObject<bool>(table.getValue(static_cast<short>(90))).getDataCopy();

    if (gameStart)
    {
        GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Game");
    }

    return;
}