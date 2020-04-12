
#include "SpecialAttack.hpp"
#include "../../CustomEventList.hpp"
#include "../Enemy/EnemyManager.hpp"
#include "Player.hpp"

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
    eManager->AllEnemysDamage(n);

    ExitGames::Common::Hashtable table;
    table.put<short, int>(DataName::Damage, n);

    networkSystem->GetClient().opRaiseEvent(true, table, CustomEvent::PlayerSpecial);
}

void SpyBomb::OnRemoteSpecial(const ExitGames::Common::Hashtable& table)
{
    const auto d = ExitGames::Common::ValueObject<int>(table.getValue(static_cast<short>(DataName::Damage))).getDataCopy();

    eManager->AllEnemysDamage(d);
}