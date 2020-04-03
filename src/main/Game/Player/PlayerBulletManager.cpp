#include "PlayerBulletManager.hpp"
#include "../../CustomEventList.hpp"
#include "Player.hpp"

constexpr double FireRate = 1.0 / 4;

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
    ExitGames::Common::Hashtable table;
    table.put<short, double>(static_cast<short>(0), pos.x);
    table.put<short, double>(static_cast<short>(1), pos.y);

    table.put<short, double>(static_cast<short>(10), spd.x);
    table.put<short, double>(static_cast<short>(11), spd.y);

    //    table.put<short, int>(static_cast<short>(99), servertime);

    networkSystem->GetClient().opRaiseEvent(false, table, CustomEvent::PlayerShot);
}

//弾を受信
void PlayerBulletManager::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::PlayerShot)
    {
        return;
    }

    //情報取得
    ExitGames::Common::Hashtable table = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent).getDataCopy();

    s3d::Vec2 pos, spd;
    pos.x = ExitGames::Common::ValueObject<double>(table.getValue(static_cast<short>(0))).getDataCopy();
    pos.y = ExitGames::Common::ValueObject<double>(table.getValue(static_cast<short>(1))).getDataCopy();
    spd.x = ExitGames::Common::ValueObject<double>(table.getValue(static_cast<short>(10))).getDataCopy();
    spd.y = ExitGames::Common::ValueObject<double>(table.getValue(static_cast<short>(11))).getDataCopy();

    //    int servertime = ExitGames::Common::ValueObject<int>(table.getValue(static_cast<short>(99))).getDataCopy();

    //ラグ計算
    // double lagtime = (networkSystem->GetServerTime() - servertime) / 1000.0;
    double lagtime = networkSystem->GetClient().getRoundTripTime() / (1000.0 * 2.0);

    pos += spd * lagtime;

    //オブジェクト生成
    auto bullet = inst();
    GetGameObject().lock()->AddChild(bullet);
    bullet->SetPosition(pos);
    auto b = bullet->GetComponent<Bullet>();
    b->moveValue = spd;
    b->lifetime -= lagtime;
}

void PlayerBulletManager::CreateBullet()
{
    // const auto playerpos = player->GetGameObject().lock()->GetLocalPosition();

    auto bullet = inst();
    GetGameObject().lock()->AddChild(bullet);

    bullet->SetPosition(player->GetGameObject().lock()->GetPosition());

    auto b = bullet->GetComponent<Bullet>();
    b->SetMove(s3d::Cursor::PosF() - player->GetGameObject().lock()->GetWorldPosition(), 110);
    b->isMine = true;

    SendBulletInfo(b);
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
