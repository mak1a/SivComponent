
#include "PlayerCore.hpp"
#include "../Bullet.hpp"
#include "UIManager.hpp"

void PlayerCore::Start2()
{
    frame = GetGameObject().lock()->GetComponent<Siv::RectFrame>();
    rect = GetGameObject().lock()->GetComponent<Siv::Rect>();
}

void PlayerCore::Update()
{
    framethickness = std::max(5, framethickness - 1);
    frame->SetInnerThickness(framethickness);

    // HPに応じて色を変化
    // rect->SetColor();
}

void PlayerCore::OnStayCollision(std::shared_ptr<GameObject>& other)
{
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

    if (life <= 0)
    {
        rect->SetColor(s3d::Palette::Black);
        uimanager->OnDefeat();
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