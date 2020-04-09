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
            text->SetText(exp[radio->GetIndex()]);
        }
    }

public:
    std::shared_ptr<Siv::RadioButton> radio;

    PlayerExplanation()
    {
        exp[0] =
            U"癖がなく扱いやすいタイプ。\n\n"
            U"どの能力も標準的で強みはないが、\n"
            U"その分復活速度が早い。\n"
            U"被弾をさほど気にせず戦える。\n";

        exp[1] =
            U"攻撃力に特化したタイプ。\n\n"
            U"高い殲滅力により、群がる敵も\n"
            U"難なく撃破していける。\n"
            U"その分弾速がやや遅く、\n"
            U"真価を引き出すには熟練が必要。";

        exp[2] =
            U"防御力に特化したタイプ。\n\n"
            U"攻撃を受け止めるタフネスさは、\n"
            U"狙いを自分に引きつける\n"
            U"大胆な立ち回りで輝く。\n"
            U"移動速度はやや遅いのが弱点。";

        exp[3] =
            U"速度に特化したタイプ。\n\n"
            U"戦場を駆け回り、狙った敵を\n"
            U"素早く仕留められる。\n"
            U"耐久がやや低いため、\n"
            U"速度を活かして攻撃を避ける\n"
            U"技量が要求される。";
    }
};