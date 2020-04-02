
#include "UIManager.hpp"

void UIManager::OnVictory()
{
    //タイマーを消す
    objects.timeUI->SetActive(false);
    //敵を全部消す
    objects.enemyManager->SetActive(false);
    //弾を出なくする
    objects.playerBulletManager->SetActive(false);
    //勝利用演出を開始
    objects.victoryUI->SetActive(true);
    //タイトルバックボタンを表示
    objects.returnTitleBt->SetActive(true);
}

void UIManager::OnDefeat()
{
    //タイマーを消す
    objects.timeUI->SetActive(false);
    //プレイヤーを全部消す
    objects.playerManager->SetActive(false);
    objects.playerBulletManager->SetActive(false);
    //敗北用演出を開始
    objects.defeatUI->SetActive(true);
    //タイトルバックボタンを表示
    objects.returnTitleBt->SetActive(true);
}