
#include "PlayerLifeView.hpp"
#include "Player.hpp"

void PlayerLifeView::Start() {}

void PlayerLifeView::Update()
{
    const int life = player->GetLife();

    text->SetText(s3d::Format(U"❤", life));
}
