#include "PlayerBulletManager.hpp"
#include "../../CustomEventList.hpp"
#include "../../Matching/Matching.hpp"
#include "Player.hpp"

constexpr double FireRate = 1.0 / 4;
constexpr int bulletspd = 180;
constexpr double range = 220;

namespace DataName
{
    constexpr nByte PosX = 0;
    constexpr nByte PosY = 1;
    constexpr nByte SpdX = 3;
    constexpr nByte SpdY = 4;
    constexpr nByte Life = 5;
}  // namespace DataName

void PlayerBulletManager::CreateBullet()
{
    // const auto playerpos = player->GetGameObject().lock()->GetLocalPosition();

    auto bullet = inst();
    GetGameObject().lock()->AddChild(bullet);

    bullet->SetPosition(player->GetGameObject().lock()->GetPosition());

    auto b = bullet->GetComponent<Bullet>();

    b->SetMove(s3d::Cursor::PosF() - player->GetGameObject().lock()->GetWorldPosition(), bulletspd);
    b->lifetime = range / bulletspd;
    b->isMine = true;

    SendBulletInfo(b);
}

void PlayerBulletManager::Start2()
{
    inst = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetInstantiate("PlayerBullet");
    fireInterval = 1.0;

    //    master = GetGameObject().lock()->GetScene().lock()->GetMasterObject();
}

//弾発射情報を送信
void PlayerBulletManager::SendBulletInfo(std::shared_ptr<Bullet>& bullet)
{
    const s3d::Vec2 pos = bullet->GetGameObject().lock()->GetLocalPosition();
    const s3d::Vec2 spd = bullet->moveValue;
    //    const int servertime = networkSystem->GetServerTime();

    //座標と発生時刻を送る
    ExitGames::Common::Dictionary<nByte, double> dic;
    dic.put(DataName::PosX, pos.x);
    dic.put(DataName::PosY, pos.y);

    dic.put(DataName::SpdX, spd.x);
    dic.put(DataName::SpdY, spd.y);

    dic.put(DataName::Life, bullet->lifetime);

    networkSystem->GetClient().opRaiseEvent(false, dic, CustomEvent::PlayerShot);
}

//弾を受信
void PlayerBulletManager::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::PlayerShot)
    {
        return;
    }

    //オブジェクト生成
    auto bullet = inst();
    auto b = bullet->GetComponent<Bullet>();

    //情報取得
    auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, double>>(eventContent).getDataCopy();

    s3d::Vec2 pos, spd;
    pos.x = *dic.getValue(DataName::PosX);
    pos.y = *dic.getValue(DataName::PosY);
    spd.x = *dic.getValue(DataName::SpdX);
    spd.y = *dic.getValue(DataName::SpdY);
    const auto life = *dic.getValue(DataName::Life);

    spd *= 1.1;

    //ラグ計算
    double lagtime = networkSystem->GetClient().getRoundTripTime() * 0.3 / 1000.0;
    pos += spd * lagtime;

    GetGameObject().lock()->AddChild(bullet);
    bullet->SetPosition(pos);

    b->moveValue = spd;
    b->lifetime = life - lagtime;
}

void PlayerBulletManager::Update()
{
    fireInterval -= s3d::Scene::DeltaTime();

    //死んでたら発射できない
    if (player->state == Player::PlayerStates::reviving)
    {
        return;
    }

    if (s3d::MouseL.pressed() && fireInterval < 0)
    {
        CreateBullet();
        fireInterval = FireRate;
    }
}
