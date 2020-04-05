
#include "PlayerCore.hpp"
#include "../../CustomEventList.hpp"
#include "../Bullet.hpp"
#include "UIManager.hpp"

namespace DataName
{
    constexpr nByte HP = 1;
}

void PlayerCore::Start2()
{
    life = maxlife;

    frame = GetGameObject().lock()->GetComponent<Siv::RectFrame>();
    rect = GetGameObject().lock()->GetComponent<Siv::Rect>();
}

void PlayerCore::Update()
{
    framethickness = std::max(5, framethickness - 1);
    frame->SetInnerThickness(framethickness);

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
    // MasterClientでなければスルー
    if (!networkSystem->IsMasterClient())
    {
        return;
    }

    if (other->GetTag() == UserDef::Tag::EnemyBullet)
    {
        auto b = other->GetComponent<Bullet>();
        life -= b->attack;
        framethickness = 20;
    }

    if (other->GetTag() == UserDef::Tag::PlayerBullet)
    {
        auto b = other->GetComponent<Bullet>();
        //フレンドリーファイア補正
        life -= (b->attack / 5);
        framethickness = 12;
    }

    //ライフがなくなったら敗北
    if (life <= 0)
    {
        //敗北信号を送る

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

// HPを受け取る
void PlayerCore::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::PlayerCoreSync)
    {
        return;
    }

    auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

    SetLife(*dic.getValue(DataName::HP));
    lastsynclife = life;
}

void PlayerCore::SetLife(int _life)
{
    life = _life;
}

int PlayerCore::GetLife() const
{
    return life;
}
