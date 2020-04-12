﻿
#include "PlayerInfoGUI.hpp"

#include "Player.hpp"

void PlayerLifeView::Start() {}

void PlayerLifeView::Update()
{
    const int life = player->GetLife();

    switch (player->GetState())
    {
        case Player::PlayerStates::normal:
            text->SetText(s3d::Format(U"â¤", life));
            break;

        case Player::PlayerStates::reviving:
            text->SetText(U"revive...");
            break;

        default:
            break;
    }
}
