
#include "Timer.hpp"
#include "../../../Matching.hpp"
#include "GameState.hpp"
#include "UIManager.hpp"

void Timer::Start()
{
    text = GetGameObject().lock()->GetComponent<Siv::Text>();

    GetGameObject()
        .lock()
        ->GetChildren()
        .front()
        ->GetComponent<Siv::Text>()
        ->SetText(U"残り時間")
        // .SetFont(s3d::Font(20, s3d::Typeface::Light))
        // .SetColor(s3d::Palette::Black)
        .SetDrawAt(true);

    watch.start();
}

void Timer::Update()
{
    auto t = GAMETIME - watch.s();
    //ゲームクリア
    if (t < 0)
    {
        uimanager->OnVictory();
    }
    text->SetText(s3d::ToString(t));
}

void TimerSetup::Start2()
{
    text = GetGameObject().lock()->GetComponent<Siv::Text>();
    text->SetDrawAt(true);

    GetGameObject()
        .lock()
        ->GetChildren()
        .front()
        ->GetComponent<Siv::Text>()
        ->SetText(U"ゲーム開始まで")
        .SetFont(s3d::Font(20, s3d::Typeface::Light))
        .SetColor(s3d::Palette::Black)
        .SetDrawAt(true);
    ;
}

void TimerSetup::Update()
{
    const auto timediff = (Matching::GameStartTime - networkSystem->GetClient().getServerTime());

    const int t = 1 + (timediff / 1000);
    text->SetText(s3d::ToString(t));

    //ゲーム中タイマーを起動して自分は機能停止
    if (timediff < 0)
    {
        s3d::Print(U"s:", Matching::GameStartTime);
        s3d::Print(U"n:", networkSystem->GetClient().getServerTime());

        timerobject->SetActive(true);
        this->SetActive(false);
        GetGameObject().lock()->GetScene().lock()->FindObject("GameSystem")->GetComponent<GameState>()->SetState(GameState::States::Playing);
    }
}