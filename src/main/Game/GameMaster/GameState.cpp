
#include "GameState.hpp"

void GameState::Start()
{
    const auto scene = GetGameObject().lock()->GetScene().lock();

    refs.enemymanager = scene->FindObject("EnemyManager");
    refs.timer = scene->FindObject("Timer");
}

void GameState::SetState(GameState::States _state)
{
    if (state == _state)
    {
        return;
    }

    state = _state;

    //状態がプレイ中に変わったら
    if (state == States::Playing)
    {
        ToPlaying();
    }
    //状態が勝利に変わったら
    if (state == States::Win)
    {
        ToWin();
    }
    //状態が敗北に変わったら
    if (state == States::Lose)
    {
        ToLose();
    }
}

void GameState::ToSetup() {}
void GameState::ToPlaying()
{
    refs.enemymanager->SetActive(true);
}
void GameState::ToWin()
{
    refs.enemymanager->SetActive(false);
    refs.timer->SetActive(false);
}
void GameState::ToLose()
{
    refs.enemymanager->SetActive(false);
    refs.timer->SetActive(false);
}
