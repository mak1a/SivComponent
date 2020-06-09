#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"

class Player;

class PlayerLifeView : public AttachableComponent
{
    void Start();
    void Update();

public:
    std::shared_ptr<Player> player;
    std::shared_ptr<Siv::Text> text;
};

// 土台
// プレイヤー番号表示
// HPバー緑
// HPバー赤
// SP保持状況の表示

class PlayerListView : public AttachableComponent
{
    double maxBarLength;
    s3d::Color backcolor;

public:
    std::shared_ptr<Player> player;
    std::shared_ptr<Siv::Rect> background;
    std::shared_ptr<Siv::Rect> greenbar;
    std::shared_ptr<Siv::Rect> spicon;
    std::shared_ptr<Siv::Text> playername;

    void Start() override;
    void Init();
    void Update() override;
};