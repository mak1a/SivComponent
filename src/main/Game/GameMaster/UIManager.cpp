
#include "UIManager.hpp"

void UIManager::Start2()
{
    GetGameObject().lock()->AddComponent<Siv::BGM>(U"BGM_Battle");
}

void UIManager::disconnectReturn()
{
    objects.playerManager->SetActive(false);
    objects.playerBulletManager->SetActive(false);
    objects.timeUI->SetActive(false);
    objects.enemyManager->SetActive(false);

    objects.connectError->SetActive(true);
    objects.returnTitleBt->SetActive(true);
}

void UIManager::OnVictory()
{
    GetGameObject().lock()->DeleteComponent<Siv::BGM>();
    s3d::AudioAsset(U"BGM_Victory").play();

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
    GetGameObject().lock()->DeleteComponent<Siv::BGM>();
    s3d::AudioAsset(U"BGM_Defeat").play();

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