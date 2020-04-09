
#include "PlayerLifeView.hpp"
#include "Player.hpp"

void PlayerLifeView::Start() {}

void PlayerLifeView::Update()
{
    const int life = player->GetLife();

    switch (player->state)
    {
        case Player::PlayerStates::normal:
            text->SetText(s3d::Format(U"❤", life));
            break;

        case Player::PlayerStates::reviving:
            text->SetText(U"revive...");
            break;

        default:
            break;
    }
}
