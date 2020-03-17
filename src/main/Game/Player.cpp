
#include "Player.hpp"
#include <Siv3D.hpp>
#include "../../CustomEventList.hpp"
#include "../../SivComponent/SivComponent.hpp"

using dictype = ExitGames::Common::Dictionary<nByte, double>;

void Player::Start2()
{
    if (isMine)
    {
        SendInstantiateMessage();
    }
}

void Player::Update()
{
    syncpos.Run();

    s3d::Point axis;
    axis.x -= s3d::KeyA.pressed();
    axis.x += s3d::KeyD.pressed();
    axis.y -= s3d::KeyW.pressed();
    axis.y += s3d::KeyS.pressed();

    auto& trans = GetGameObject().lock()->transform();
    auto pos = trans.GetPosition();
    pos += axis * 5;
    trans.SetPosition(pos);
}

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

void Player::SyncPos()
{
    // 自己位置の送信
    dictype dic;

    auto pos = GetGameObject().lock()->transform().GetPosition();
    dic.put(DataName::Player::posX, pos.x);
    dic.put(DataName::Player::posY, pos.y);

    networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::PlayerSync);
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
