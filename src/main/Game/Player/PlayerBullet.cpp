

#include "PlayerBullet.hpp"
#include "../../../CustomEventList.hpp"

void BulletManager::Start2()
{
    inst = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetInstantiate("PlayerBullet");

    //    master = GetGameObject().lock()->GetScene().lock()->GetMasterObject();
}

//弾発射情報を送信
void BulletManager::SendBulletInfo(std::shared_ptr<Bullet>& bullet)
{
    const s3d::Vec2 pos = bullet->GetGameObject().lock()->GetLocalPosition();
    const s3d::Vec2 spd = bullet->moveValue;
    const int servertime = networkSystem->GetServerTime();

    //座標と発生時刻を送る
    ExitGames::Common::Hashtable table;
    table.put<short, double>(static_cast<short>(0), pos.x);
    table.put<short, double>(static_cast<short>(1), pos.y);

    table.put<short, double>(static_cast<short>(10), spd.x);
    table.put<short, double>(static_cast<short>(11), spd.y);

    table.put<short, int>(static_cast<short>(99), servertime);

    networkSystem->GetClient().opRaiseEvent(true, table, CustomEvent::PlayerShot);
}

//弾を受信
void BulletManager::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
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

    int servertime = ExitGames::Common::ValueObject<int>(table.getValue(static_cast<short>(99))).getDataCopy();

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

void BulletManager::CreateBullet()
{
    // const auto playerpos = player->GetGameObject().lock()->GetLocalPosition();

    auto bullet = inst();
    GetGameObject().lock()->AddChild(bullet);

    bullet->SetPosition(player->GetGameObject().lock()->GetPosition());

    auto b = bullet->GetComponent<Bullet>();
    b->SetMove(s3d::Cursor::PosF() - player->GetGameObject().lock()->GetWorldPosition(), 110);

    // s3d::Print(U"Player:", playerpos);
    // s3d::Print(U"Cursor:", s3d::Cursor::PosF());
    // s3d::Print(U"Target:", master->transform().GetMatrix().inversed().transform(s3d::Cursor::PosF()));
    // s3d::Print(U"offset loal:", s3d::Cursor::PosF() - player->GetGameObject().lock()->GetLocalPosition());
    // s3d::Print(U"offset world:", s3d::Cursor::PosF() - player->GetGameObject().lock()->GetWorldPosition());

    b->isMine = true;

    SendBulletInfo(b);
}

void BulletManager::Update()
{
    if (s3d::MouseL.down())
    {
        CreateBullet();
    }
}

//----

void Bullet::Update()
{
    auto obj = GetGameObject().lock();
    auto pos = obj->GetPosition();
    obj->SetPosition(pos.moveBy(moveValue * s3d::Scene::DeltaTime()));

    lifetime -= s3d::Scene::DeltaTime();
    if (lifetime < 0)
    {
        auto obj = GetGameObject().lock();
        obj->GetScene().lock()->Destroy(obj);
    }
}

void Bullet::SetMove(const s3d::Vec2& target, double speed)
{
    moveValue = target.normalized() * speed;
    // moveValue = GetGameObject().lock()->LookAt(target) * speed;
}
