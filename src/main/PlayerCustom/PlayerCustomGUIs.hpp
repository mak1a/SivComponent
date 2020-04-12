#pragma once

#include "../../ComponentEngine/ComponentEngine.hpp"

// U"標準型", U"攻撃型", U"耐久型", U"速度型"

// class PlayerDifficulty : public AttachableComponent
// {
//     std::shared_ptr<Siv::Text> text;
//     std::shared_ptr<Siv::RadioButton> radio;

//     const std::array<s3d::String, 4> exp = {U"★", U"★★★", U"★★", U"★"};

//     void Start()
//     {
//         text = GetGameObject().lock()->GetComponent<Siv::Text>();
//         radio.GetIndex();
//     }

//     void Update() {}
// };

class PlayerExplanation : public AttachableComponent
{
    std::shared_ptr<Siv::Text> text;

    std::array<s3d::String, 4> exp = {U"★", U"★★★", U"★★", U"★"};

    void Start()
    {
        text = GetGameObject().lock()->GetComponent<Siv::Text>();
        text->SetText(exp[radio->GetIndex()]);
    }

    void Update()
    {
        if (radio->Changed())
        {
            s3d::AudioAsset(U"Menu").playOneShot(0.3);
            text->SetText(exp[radio->GetIndex()]);
        }
    }

public:
    std::shared_ptr<Siv::RadioButton> radio;

    PlayerExplanation();
};

class PlayerTypeSave : public AttachableComponent
{
    std::shared_ptr<Siv::RadioButton> radio;

    void Start();

    void Update();
};
