
#include "PlayerCore.hpp"
#include "../../Common/CommonMemory.hpp"
#include "../../CustomEventList.hpp"
#include "../../Matching/Matching.hpp"
#include "../Bullet.hpp"
#include "GameSync.hpp"
#include "UIManager.hpp"

namespace DataName
{
    constexpr nByte HP = 1;
}

void PlayerCore::Start2()
{
    constexpr int basevalue[] = {700, 800, 1500, 2000, 3000};

    maxlife = basevalue[CommonMemory::GetDifficulty()];
    life = maxlife;

    frame = GetGameObject().lock()->GetComponent<Siv::RectFrame>();
    rect = GetGameObject().lock()->GetComponent<Siv::Rect>();

    framethickness = 5;
}

void PlayerCore::Update()
{
    framethickness = std::max(5, framethickness - 1);
    frame->SetInnerThickness(framethickness);

    // s3d::Print(U"framethickness", framethickness);

    // HPに応じて色を変化
    // rect->SetColor();

    //最後に送信したHPと違ったらダメージを受けたと判断
    const bool damaged = life != lastsynclife;

    //定期的にHP同期信号を送る
    if (damaged && networkSystem->IsMasterClient())
    {
        hpSync.Run();
        lastsynclife = life;
    }
}

void PlayerCore::OnStayCollision(std::shared_ptr<GameObject>& other)
{
    if (other->GetTag() == UserDef::Tag::EnemyBullet || other->GetTag() == UserDef::Tag::PlayerBullet)
    {
        //被弾音
        s3d::AudioAsset(U"CoreDamage").playOneShot(0.2);
    }

    if (other->GetTag() == UserDef::Tag::EnemyBullet)
    {
        auto b = other->GetComponent<Bullet>();
        framethickness = 20;

        // MasterClientでなければスルー
        if (!networkSystem->IsMasterClient())
        {
            return;
        }

        life -= b->attack;
    }

    if (other->GetTag() == UserDef::Tag::PlayerBullet)
    {
        auto b = other->GetComponent<Bullet>();
        //フレンドリーファイア補正
        framethickness = 12;

        // MasterClientでなければスルー
        if (!networkSystem->IsMasterClient())
        {
            return;
        }

        life -= (b->attack / 5);
    }

    //ライフがなくなったら敗北
    if (life <= 0)
    {
        //敗北信号を送る
        SendDefeatSignal();

        OnDefeat();
    }
}

// HPを同期
void PlayerCore::PlayerCoreSync()
{
    ExitGames::Common::Dictionary<nByte, int> dic;

    dic.put(DataName::HP, life);

    // PlayerManagerが受け取る
    networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::PlayerCoreSync);
}

void PlayerCore::OnDefeat()
{
    rect->SetColor(s3d::Palette::Black);
    uimanager->OnDefeat();
}

void PlayerCore::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    // HP同期
    if (eventCode == CustomEvent::PlayerCoreSync)
    {
        auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

        SetLife(*dic.getValue(DataName::HP));
        lastsynclife = life;
    }

    //敗北信号
    if (eventCode == CustomEvent::GameResult)
    {
        auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

        const auto result = *dic.getValue(DataName::GameSync::Result);

        if (result == static_cast<int>(DataName::GameSync::ResultType::Defeat))
        {
            OnDefeat();
        }
    }
}

void PlayerCore::SetLife(int _life)
{
    life = _life;
}

int PlayerCore::GetLife() const
{
    return life;
}
