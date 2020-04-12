
#include "SpecialAttack.hpp"
#include "../../CustomEventList.hpp"
#include "../Enemy/EnemyManager.hpp"
#include "../GameMaster/PlayerCore.hpp"
#include "Player.hpp"
#include "PlayerManager.hpp"

void ISpecialAttack::Start2()
{
    playerAnime = GetGameObject().lock()->GetComponent<PlayerAnimation>();

    player = GetGameObject().lock()->GetComponent<Player>();

    Start3();
}

void ISpecialAttack::Special()
{
    if (!hassp)
    {
        return;
    }

    hassp = false;
    playerAnime->OnBomb();
    s3d::AudioAsset(U"PlayerSpecial").playOneShot(0.5);
    OnSpecial();
}

void ISpecialAttack::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::PlayerSpecial)
    {
        return;
    }

    if (playerNr != player->GetPlayerNr())
    {
        return;
    }
    // playerNrが同一ならボムも同一なので確認する必要なし

    ExitGames::Common::Hashtable table = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent).getDataCopy();

    hassp = false;
    playerAnime->OnBomb();
    s3d::AudioAsset(U"PlayerSpecial").playOneShot(0.5);
    OnRemoteSpecial(table);
}

void SpyBomb::Start3()
{
    eManager = GetGameObject().lock()->GetScene().lock()->FindObject("EnemyManager")->GetComponent<EnemyManager>();
}

void SpyBomb::OnSpecial()
{
    // constexpr double dam[] = {10.0, 5.0, 1.0, 0.8, 0.2};

    //弾を消す
    auto n = eManager->DestroyAllBullets();

    if (!player->IsMine())
    {
        return;
    }

    //ダメージ
    const int dmg = 10 + (n / 2);
    for (auto& e : eManager->GetAllEnemyMaster()->GetChildren())
    {
        e->GetComponent<Enemy>()->DamageAndSend(dmg);
    }

    ExitGames::Common::Hashtable table;
    table.put<short, int>(DataName::Damage, n);

    networkSystem->GetClient().opRaiseEvent(true, table, CustomEvent::PlayerSpecial);
}

void SpyBomb::OnRemoteSpecial(const ExitGames::Common::Hashtable& table)
{
    eManager->DestroyAllBullets();
}

//=============

void AttackSpecial::Start3()
{
    info.reset(new Player::FireInfo());

    //初期値を取っておく
    *info = player->GetFireInfo();
}

void AttackSpecial::OnSpecial()
{
    enable = true;

    auto powered = player->GetFireInfo();

    powered.attack *= 5;
    powered.speed *= 2.8;
    powered.lifetime *= 0.65;

    time = 8;

    player->SetFireInfo(powered);

    //受信データだったら何もしない
    if (player->GetPlayerNr() != networkSystem->GetClient().getLocalPlayer().getNumber())
    {
        return;
    }

    ExitGames::Common::Hashtable table;

    networkSystem->GetClient().opRaiseEvent(true, table, CustomEvent::PlayerSpecial);
}

void AttackSpecial::OnRemoteSpecial(const ExitGames::Common::Hashtable& table)
{
    OnSpecial();
}

void AttackSpecial::Update()
{
    if (enable)
    {
        time -= s3d::Scene::DeltaTime();

        //死んだら終了
        if (player->GetState() == Player::PlayerStates::reviving)
        {
            time -= 100;
        }

        if (time < 0)
        {
            enable = false;
            //ステータスを元に戻す
            player->SetFireInfo(*info);
        }
    }
}

//=============

void DefenceSpecial::Start3()
{
    pManager = GetGameObject().lock()->GetScene().lock()->FindObject("PlayerManager")->GetComponent<PlayerManager>();
    core = GetGameObject().lock()->GetScene().lock()->FindObject("PlayerCore")->GetComponent<PlayerCore>();
}

void DefenceSpecial::OnSpecial()
{
    for (auto& p : pManager->players)
    {
        p->OnRevive();

        const auto life = p->GetLife() * 5;
        p->SetLife(life);
    }

    const auto current = core->GetLife();
    const auto plus = core->GetMaxLife() * 0.25;
    auto value = std::min<int>(current + plus, core->GetMaxLife());
    core->SetLife(value);

    //受信データだったら何もしない
    if (player->GetPlayerNr() != networkSystem->GetClient().getLocalPlayer().getNumber())
    {
        return;
    }

    ExitGames::Common::Hashtable table;

    networkSystem->GetClient().opRaiseEvent(true, table, CustomEvent::PlayerSpecial);
}

void DefenceSpecial::OnRemoteSpecial(const ExitGames::Common::Hashtable& table)
{
    OnSpecial();
}

//=============

void SpeedSpecial::Start3()
{
    info.reset(new Player::FireInfo());

    //初期値を取っておく
    *info = player->GetFireInfo();
    speed = player->GetSpeed();
}

void SpeedSpecial::OnSpecial()
{
    enable = true;

    auto powered = player->GetFireInfo();

    powered.speed *= 3.0;
    powered.lifetime *= 5.0;
    const auto spd = player->GetSpeed() * 2.5;
    player->SetSpeed(spd);

    time = 18;

    player->SetFireInfo(powered);

    //受信データだったら何もしない
    if (player->GetPlayerNr() != networkSystem->GetClient().getLocalPlayer().getNumber())
    {
        return;
    }

    ExitGames::Common::Hashtable table;

    networkSystem->GetClient().opRaiseEvent(true, table, CustomEvent::PlayerSpecial);
}

void SpeedSpecial::OnRemoteSpecial(const ExitGames::Common::Hashtable& table)
{
    OnSpecial();
}

void SpeedSpecial::Update()
{
    if (enable)
    {
        time -= s3d::Scene::DeltaTime();

        //死んだら終了
        if (player->GetState() == Player::PlayerStates::reviving)
        {
            time -= 100;
        }

        if (time < 0)
        {
            enable = false;
            //ステータスを元に戻す
            player->SetFireInfo(*info);
            player->SetSpeed(speed);
        }
    }
}
