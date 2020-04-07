
#include "Timer.hpp"
#include "../../Matching/Matching.hpp"
#include "GameState.hpp"
#include "GameSync.hpp"
#include "UIManager.hpp"

void Timer::Start2()
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
    auto t = GetTime();

    //ゲームクリア
    if (t < 0 && networkSystem->IsMasterClient())
    {
        //信号を送信
        SendVictorySignal();
        uimanager->OnVictory();
    }

    text->SetText(s3d::ToString(std::max(0, t)));
}

void Timer::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    //勝利信号
    if (eventCode != CustomEvent::GameResult)
    {
        return;
    }

    auto dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

    const auto result = *dic.getValue(DataName::GameSync::Result);

    if (result == static_cast<int>(DataName::GameSync::ResultType::Victory))
    {
        uimanager->OnVictory();
    }
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
    // s3d::Print(U"s:", Matching::GameStartTime);
    // s3d::Print(U"n:", networkSystem->GetServerTime());
}

void TimerSetup::Update()
{
    const auto timediff = (Matching::GameStartTime - networkSystem->GetClient().getServerTime());

    const int t = 1 + (timediff / 1000);
    text->SetText(s3d::ToString(t));

    //ゲーム中タイマーを起動して自分は機能停止
    if (timediff < 0)
    {
        timerobject->SetActive(true);
        this->SetActive(false);
        GetGameObject().lock()->GetScene().lock()->FindObject("GameSystem")->GetComponent<GameState>()->SetState(GameState::States::Playing);

        explanation->SetActive(false);
    }

    // HACK: これはサーバータイムバグに対するその場しのぎの対策
    if (timediff > 10 * 1000)
    {
        Matching::GameStartTime = networkSystem->GetServerTime() + 5000;
    }
}
