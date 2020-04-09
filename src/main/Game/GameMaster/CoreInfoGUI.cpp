#include "CoreInfoGUI.hpp"
#include "PlayerCore.hpp"

void CoreInfoGUI::Start()
{
    maxBarWidth = greenbar->GetShape().w;
}

void CoreInfoGUI::Update()
{
    //割合算出
    const double hp = (double)core->GetLife() / core->GetMaxLife();

    // HPバーの現在位置を計算する
    //減少は一瞬で、上昇はゆっくりと
    greenprogress = std::clamp(hp, 0.0, std::min(1.0, greenprogress + s3d::Scene::DeltaTime() * 0.25));

    //赤ゲージの位置を計算する
    //減少はゆっくりと、上昇は一瞬で
    redprogress = std::clamp(greenprogress, redprogress - s3d::Scene::DeltaTime() * 0.05, 1.0);

    //バーの座標を計算する
    // greenprogressの割合分だけ長さを短くする
    auto shape = greenbar->GetShape();
    shape.w = maxBarWidth * greenprogress;
    greenbar->SetShape(shape);

    shape = redbar->GetShape();
    shape.w = maxBarWidth * redprogress;
    redbar->SetShape(shape);
}