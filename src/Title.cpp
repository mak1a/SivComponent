
#include "Title.hpp"

using namespace ComponentEngine;

void TitleScene::Setup()
{
    s3d::Scene::SetBackground(Palette::Skyblue);

    auto obj = CreateAndGetObject();
    obj->transform().SetPosition(s3d::Scene::Center());
    obj->AddComponent<ComponentEngine::Siv::TextBox>()->SetMaxLength(20).SetText(U"オンラインCoopゲーム(仮題 α版)");

    auto title = CreateAndGetObject();
    auto t = title->AddComponent<Siv::Text>();
    t->SetFont(Font(50)).SetColor(Palette::Black);
}
