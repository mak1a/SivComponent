
#include "PlayerInfoGUI.hpp"
#include "Player.hpp"
#include "PlayerManager.hpp"
#include "SpecialAttack.hpp"

#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "../../../Utilities/PhotonUtilities.hpp"

void PlayerLifeView::Start() {}

void PlayerLifeView::Update()
{
    const int life = player->GetLife();

    switch (player->GetState())
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

void PlayerListView::Start()
{
    auto net = GetGameObject().lock()->GetScene().lock()->GetSceneManager()->GetCommon().GetCommonObject(Photon::NetworkObjectName())->GetComponent<Photon::NetworkSystem>();
    const auto& lists = net->GetPlayerList();

    for (const auto& l : lists)
    {
        if (l->getNumber() != player->GetPlayerNr())
        {
            continue;
        }

        int num = Utilities::GetPlayerNumber(l->getName());

        // auto name = ;
        playername->SetText(s3d::Format(U"Player", num));

        break;
    }

    maxBarLength = greenbar->GetShape().w;

    backcolor = PlayerDef::colors[(int)player->GetType()];
    backcolor = backcolor.lerp(s3d::Palette::Gray, 0.7);
    backcolor.a = 200;
    background->SetColor(backcolor);
}

void PlayerListView::Update()
{
    //バーの更新
    auto bar = greenbar->GetShape();
    bar.w = s3d::Clamp<double>(maxBarLength * ((double)player->GetLife() / player->GetMaxLife()), 0.0, maxBarLength);
    greenbar->SetShape(bar);

    if (player->GetState() == Player::PlayerStates::reviving)
    {
        background->SetColor(s3d::Color(40, 40, 40, 200));
    }
    else
    {
        background->SetColor(backcolor);
    }

    spicon->SetActive(player->specialAttack->HasSP());
}
