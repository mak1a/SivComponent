#pragma once

#include "CoreInfoGUI.hpp"

#include "../../../ComponentEngine/ComponentEngine.hpp"

class PlayerCore;

class CoreInfoGUI : public AttachableComponent
{
    void Start() override;
    void Update() override;

    //赤ゲージ表示用、見た目のHPバーの割合 0 ~ 1
    double greenprogress = 0, redprogress = 0;

    double maxBarWidth;

public:
    // HPバー
    std::shared_ptr<Siv::Rect> greenbar;
    std::shared_ptr<Siv::Rect> redbar;

    //参照
    std::shared_ptr<PlayerCore> core;
};