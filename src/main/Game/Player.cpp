
#include "Player.hpp"
#include "../../CustomEventList.hpp"

using dictype = ExitGames::Common::Dictionary<nByte, double>;

void Player::Start()
{
    // 自己位置の送信
    dictype dic;

    dic.put(DataName::Player::playerNr, networkSystem->GetClient().getLocalPlayer().getNumber());
    dic.put(DataName::Player::posX, 100 + networkSystem->GetPlayerNumberInRoom() * 100);
    dic.put(DataName::Player::posY, 300);

    networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::PlayerInit);
}

void Player::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::PlayerInit)
    {
        return;
    }

    ExitGames::Common::Dictionary<nByte, double> dic =
        ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, double> >(eventContent).getDataCopy();

    s3d::Print(U"receive value");
    double x = *dic.getValue(DataName::Player::posX);
    s3d::Print(x);
}
