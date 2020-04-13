
#include "PlayerCustomGUIs.hpp"
#include "../Common/CommonMemory.hpp"

PlayerExplanation::PlayerExplanation()
{
    exp[0] =
        U"癖がなく扱いやすいタイプ。\n\n"
        U"どの能力も標準的で強みはないが、\n"
        U"その分復活速度が早い。\n"
        U"被弾をさほど気にせず戦える。\n\n"
        U"スペシャル：スパイボム\n"
        U"全ての敵弾を消し、量に応じた全体ダメージ。";

    exp[1] =
        U"攻撃力に特化したタイプ。\n\n"
        U"群がる敵も難なく殲滅していける。\n"
        U"その分弾速がやや遅く、当てづらい。\n"
        U"真価を引き出すには熟練が必要。\n\n"
        U"スペシャル：フルブースト\n"
        U"一定時間自身の射撃を大幅に強化する。";

    exp[2] =
        U"防御力に特化したタイプ。\n\n"
        U"攻撃を正面から受け止めるタフネスさは、\n"
        U"狙いを自分に引きつける大胆な立ち回りで輝く。\n"
        U"移動速度はやや遅いのが弱点。\n\n"
        U"スペシャル：グランドウォール\n"
        U"コアの耐久を回復させ、味方の耐久を限界突破。";

    exp[3] =
        U"速度に特化したタイプ。\n\n"
        U"戦場を駆け回り、狙った敵を素早く仕留められる。\n"
        U"耐久がやや低く射程も短いため、\n"
        U"速度を活かして攻撃を避ける技量が要求される。\n\n"
        U"スペシャル：コメットドライブ\n"
        U"一定時間速度が超上昇する";
}

void PlayerTypeSave::Start()
{
    radio = GetGameObject().lock()->GetComponent<Siv::RadioButton>();
    radio->SetIndex(CommonMemory::GetPlayerType());
}

void PlayerTypeSave::Update()
{
    CommonMemory::SetPlayerType(static_cast<int>(radio->GetIndex()));
}
