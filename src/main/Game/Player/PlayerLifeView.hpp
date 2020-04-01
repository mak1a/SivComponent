
#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp";

class Player;

class PlayerLifeView : public AttachableComponent
{
    void Start();
    void Update();

public:
    std::shared_ptr<Player> player;
    std::shared_ptr<Siv::Text> text;
};