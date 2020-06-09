
#include "AssetLoad.hpp"
#include <Siv3D.hpp>

void LoadAssets()
{
    constexpr double masterVolume = 0.06;

    AudioAsset::Register(U"BGM_Title", U"Assets/Audio/BGM/Title.mp3");
    AudioAsset::Register(U"BGM_Custom", U"Assets/Audio/BGM/PlayerCustomBGM.mp3");
    AudioAsset::Register(U"BGM_Matching", U"Assets/Audio/BGM/matchingBGM.mp3");
    AudioAsset::Register(U"BGM_Battle", U"Assets/Audio/BGM/battle1.mp3");
    AudioAsset::Register(U"BGM_Victory", U"Assets/Audio/SE/victory.mp3");
    AudioAsset::Register(U"BGM_Defeat", U"Assets/Audio/BGM/defeatBGM.mp3");

    AudioAsset(U"BGM_Title").setVolume(masterVolume);
    AudioAsset(U"BGM_Custom").setVolume(masterVolume * 2.8);
    AudioAsset(U"BGM_Matching").setVolume(masterVolume * 1.8);
    AudioAsset(U"BGM_Battle").setVolume(masterVolume);
    AudioAsset(U"BGM_Victory").setVolume(masterVolume);
    AudioAsset(U"BGM_Defeat").setVolume(masterVolume);

    AudioAsset(U"BGM_Title").setLoop(true);
    AudioAsset(U"BGM_Custom").setLoop(true);
    AudioAsset(U"BGM_Matching").setLoop(true);
    AudioAsset(U"BGM_Battle").setLoop(true);

    // SE
    AudioAsset::Register(U"OK", U"Assets/Audio/SE/OK.mp3");
    AudioAsset::Register(U"NG", U"Assets/Audio/SE/NG.mp3");
    AudioAsset::Register(U"Menu", U"Assets/Audio/SE/Menu.mp3");
    AudioAsset::Register(U"Countdown", U"Assets/Audio/SE/countdown.mp3");
    AudioAsset::Register(U"GameStart", U"Assets/Audio/SE/gamestart.mp3");
    AudioAsset::Register(U"CoreDamage", U"Assets/Audio/SE/coreDamage.mp3");
    AudioAsset::Register(U"PlayerDamage", U"Assets/Audio/SE/playerDamage.mp3");
    AudioAsset::Register(U"PlayerShot", U"Assets/Audio/SE/playerShot.mp3");
    AudioAsset::Register(U"EnemyShot", U"Assets/Audio/SE/enemyShot.mp3");
    AudioAsset::Register(U"EnemyDamage", U"Assets/Audio/SE/bulletHit.mp3");
    AudioAsset::Register(U"EnemyDeath", U"Assets/Audio/SE/enemyDestroy.mp3");
    AudioAsset::Register(U"PlayerDeath", U"Assets/Audio/SE/playerDeath.mp3");
    AudioAsset::Register(U"GenerateEnemy", U"Assets/Audio/SE/enemyEntry.mp3");
    AudioAsset::Register(U"PlayerRevive", U"Assets/Audio/SE/playerRevive.mp3");
    AudioAsset::Register(U"PlayerSpecial", U"Assets/Audio/SE/playerSpecial.mp3");

    AudioAsset(U"OK").setVolume(masterVolume);

    AudioAsset(U"NG").setVolume(masterVolume);
    AudioAsset(U"Menu").setVolume(masterVolume);
    AudioAsset(U"Countdown").setVolume(masterVolume);
    AudioAsset(U"GameStart").setVolume(masterVolume * 2);
    AudioAsset(U"CoreDamage").setVolume(masterVolume);
    AudioAsset(U"PlayerDamage").setVolume(masterVolume);
    AudioAsset(U"PlayerShot").setVolume(masterVolume);
    AudioAsset(U"EnemyShot").setVolume(masterVolume);
    AudioAsset(U"EnemyDamage").setVolume(masterVolume);
    AudioAsset(U"EnemyDeath").setVolume(masterVolume);
    AudioAsset(U"PlayerDeath").setVolume(masterVolume);
    AudioAsset(U"GenerateEnemy").setVolume(masterVolume);
    AudioAsset(U"PlayerRevive").setVolume(masterVolume);
    AudioAsset(U"PlayerSpecial").setVolume(masterVolume);
}