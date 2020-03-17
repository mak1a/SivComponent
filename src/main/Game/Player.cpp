
#include "Player.hpp"
#include "../../CustomEventList.hpp"
#include "../../SivComponent/SivComponent.hpp"

using dictype = ExitGames::Common::Dictionary<nByte, double>;

void Player::SendInstantiateMessage()
{
    // 自己位置の送信
    dictype dic;

    auto pos = GetGameObject().lock()->transform().GetPosition();
    dic.put(DataName::Player::posX, pos.x);
    dic.put(DataName::Player::posY, pos.y);

    networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::PlayerInit);

    if (isMine)
    {
        GetGameObject().lock()->GetComponent<Siv::Circle>()->SetColor(s3d::Palette::Orange);
    }
}

s3d::Vec2 Player::playerInitpos[4] = {s3d::Vec2(200, 200), s3d::Vec2(400, 200), s3d::Vec2(200, 400), s3d::Vec2(400, 400)};

void PlayerCreator::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::PlayerInit)
    {
        return;
    }

    ExitGames::Common::Dictionary<nByte, double> dic =
        ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, double> >(eventContent).getDataCopy();

    double x = *dic.getValue(DataName::Player::posX);
    double y = *dic.getValue(DataName::Player::posY);
    auto obj = GetGameObject().lock();
    auto otherplayer = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().Instantiate("Player", obj);

    otherplayer->transform().SetPosition({x, y});

    otherplayer->GetComponent<Player>()->playerNr = playerNr;
}