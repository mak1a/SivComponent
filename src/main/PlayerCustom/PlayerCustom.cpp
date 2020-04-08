
#include "PlayerCustom.hpp"
#include "../Common/Buttons.hpp"

void PlayerCustom::Setup()
{
    auto bt = CreateAndGetObject();
    bt->SetPosition({200, s3d::Scene::Height() - 60});
    auto csb = bt->AddComponent<ChangeSceneButton>("Title");
    bt->AddComponent<Siv::Button>()->SetDelegate([=]() { csb->OnClick(); }).SetText(U"タイトルへ");

    auto toMatchBt = CreateAndGetObject();
    toMatchBt->SetPosition({s3d::Scene::Width() - 200, s3d::Scene::Height() - 60});
    auto csb2 = toMatchBt->AddComponent<ChangeSceneButton>("Matching");
    toMatchBt->AddComponent<Siv::Button>()->SetDelegate([=]() { csb2->OnClick(); }).SetText(U"マッチング");
}
