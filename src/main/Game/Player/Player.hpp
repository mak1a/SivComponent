#pragma once

#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "../../../Utilities/IntervalCall.hpp"
#include "../../Common/CommonMemory.hpp"

class PlayerAnimation : public AttachableComponent
{
    std::shared_ptr<Siv::RectFrame> frame;
    s3d::ColorF basecolor;
    double time = 0;
    bool anime = false;

    void Start() override;
    void Update() override;

public:
    void OnBomb();
};

class ISpecialAttack;

namespace PlayerDef
{
    // Standard, Attack, Defence, Speed,
    constexpr std::array<s3d::Color, 4> colors = {s3d::Color(0, 255, 0), s3d::Color(255, 0, 0), s3d::Color(0, 0, 255), s3d::Color(255, 255, 0)};
}  // namespace PlayerDef

class Player : public Photon::AttachableComponentPhotonCallbacks
{
public:
    enum PlayerStates
    {
        normal,   //基本状態
        reviving  //復活中
    };

    struct FireInfo
    {
        int attack;
        double lifetime;
        int speed;
        int spread = 1;
    };

private:
    void Start2() override;
    void Update() override;
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;
    void OnStayCollision(std::shared_ptr<GameObject>& other) override;

    void OnWall();
    void OnEnemy();
    void OnEnemyBullet(std::shared_ptr<GameObject>& other);
    void Move();
    void Regenerate();
    void Reviveing();

    bool isMine = false;

    double spd;
    int maxlife;
    int life;

    FireInfo fire;

    //リジェネの回復実数値
    double regene = 0;
    //リジェネの回復加速度
    double regenespd = 0;

    //蘇生にかかる時間
    double reviveCost;
    //蘇生状況
    double revivetimer = 0;
    PlayerType type;

    //線形補間用
    s3d::Vec2 targetPos;
    s3d::Vec2 movehistory;

    Utilities::IntervalCall syncstatus;
    std::shared_ptr<GameObject> camera;

    std::shared_ptr<Siv::Rect> rect;
    s3d::Color defaultcolor;

    PlayerStates m_state;

    int playerNr;

public:
    void SetFireInfo(const FireInfo& info)
    {
        fire = info;
    }

    const FireInfo& GetFireInfo()
    {
        return fire;
    }

    void SetSpeed(double speed)
    {
        spd = speed;
    }

    double GetSpeed()
    {
        return spd;
    }

    std::shared_ptr<ISpecialAttack> specialAttack;
    std::shared_ptr<Siv::Circle> centerCircle;

    void SetPlayerNr(int number)
    {
        playerNr = number;
    }

    int GetPlayerNr()
    {
        return playerNr;
    }

    PlayerStates GetState() const
    {
        return m_state;
    }

    void SetState(PlayerStates _state);

    bool IsMine() const noexcept
    {
        return isMine;
    }

    void SetMine(bool ismine)
    {
        isMine = ismine;
    }

    int GetLife() const
    {
        return life;
    }

    //限界突破用
    void SetLife(int hp)
    {
        life = hp;
    }

    int GetMaxLife()
    {
        return maxlife;
    }

    PlayerType GetType() const
    {
        return type;
    }
    //強制的に蘇生も可能
    void OnRevive();

    //タイプを設定し、それにより数値を変更
    void SetType(PlayerType _type);

    void SendInstantiateMessage();
    void SyncStatus();

    Player();

private:
    void SyncPosWithEasing();

public:
    //プレイヤーの位置初期化用配列
    static s3d::Vec2 playerInitpos[4];
};

namespace DataName::Player
{
    constexpr nByte posX = 1;
    constexpr nByte posY = 2;
    constexpr nByte Life = 3;
    constexpr nByte Spd = 4;
    constexpr nByte CurrentState = 5;
    constexpr nByte Type = 6;
};  // namespace DataName::Player