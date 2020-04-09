﻿
#include "PlayerCustom.hpp"
#include "../Common/Buttons.hpp"
#include "PlayerCustomGUIs.hpp"

void PlayerCustom::Setup()
{
    s3d::Scene::SetBackground(s3d::Palette::Lightskyblue);

    //タイトル
    auto title = CreateAndGetObject();
    title->AddComponent<Siv::Text>()->SetText(U"プレイヤーのタイプを選択してください").SetColor(s3d::Palette::Black).SetFont(s3d::Font(30, s3d::Typeface::Bold)).SetDrawAt(false);
    title->SetPosition({10, 10});

    //ラジオボタン
    auto radio = CreateAndGetObject();
    const s3d::Array<s3d::String> sssssssssssss = {U"標準型", U"攻撃型", U"耐久型", U"速度型"};
    const auto radioComp = radio->AddComponent<Siv::RadioButton>(sssssssssssss);
    radio->SetPosition({20, 70});

    //説明文
    auto explanation = CreateAndGetObject();
    explanation->AddComponent<Siv::Text>()->SetDrawAt(false).SetColor(s3d::Palette::Black);
    explanation->AddComponent<PlayerExplanation>()->radio = radioComp;
    explanation->SetPosition({180, 100});

    auto bt = CreateAndGetObject();
    bt->SetPosition({200, s3d::Scene::Height() - 60});
    auto csb = bt->AddComponent<ChangeSceneButton>("Title");
    bt->AddComponent<Siv::Button>()->SetDelegate([=]() { csb->OnClick(); }).SetText(U"タイトルへ");

    auto toMatchBt = CreateAndGetObject();
    toMatchBt->SetPosition({s3d::Scene::Width() - 200, s3d::Scene::Height() - 60});
    auto csb2 = toMatchBt->AddComponent<ChangeSceneButton>("Matching");
    toMatchBt->AddComponent<Siv::Button>()->SetDelegate([=]() { csb2->OnClick(); }).SetText(U"マッチング");
}
