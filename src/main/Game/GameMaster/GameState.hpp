#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"

//ゲーム全体の場面を管理する
//そのうちウェーブ制にしたい
class GameState : public AttachableComponent
{
public:
    enum States
    {
        Setup,    //開始前
        Playing,  //プレイ中
        Win,      //クリア
        Lose,     //失敗
    };

    States GetState() const
    {
        return state;
    }
    void SetState(States _state);

    struct
    {
        std::shared_ptr<GameObject> enemymanager, timer;
    } refs;

private:
    States state;

    void Start();
    void Update()
    {
        if (s3d::KeySpace.down())
        {
            SetState(States::Win);
        }
    }

    void ToSetup();
    void ToPlaying();
    void ToWin();
    void ToLose();
};