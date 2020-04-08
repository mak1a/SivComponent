
#include "Enemy.hpp"
#include "../../../Utilities/Utilities.hpp"
#include "../../CustomEventList.hpp"
#include "../../Matching/Matching.hpp"
#include "../Bullet.hpp"
#include "../Player/Player.hpp"
#include "../Player/PlayerManager.hpp"
#include "EnemyManager.hpp"

static int enemynumber;

int GenerateEnemyNumber()
{
    return enemynumber++;
}

void ResetEnemyNumber()
{
    enemynumber = ComponentEngine::Photon::NetworkSystem::GetInstance()->GetPlayerNumberInRoom() * 100000;
}

int MakeSpeed();
int MakeLife();
int MakeFirespread();
int MakeFireattack();
int MakeFirespeed();
int MakeFirelife();

//難易度から自動生成
void Enemy::GenerateStatus()
{
    //この関数を利用するのは自分のオブジェクト生成時のみなので、isMineをセット
    isMine = true;
    //座標生成

    //新アルゴリズム
    constexpr int size = 950 - 300;
    const auto frame = s3d::Rect(-size, -size, size * 2, size * 2).stretched(s3d::Random() * 80);
    s3d::Circular3 c;
    c.r = 10000000;
    c.theta = s3d::Random(2 * s3d::Math::Pi);
    const auto line = s3d::Line({0, 0}, c);

    const s3d::Vec2 pos = frame.intersectsAt(line).value()[0];  // s3d::Vec2(length, 0).rotate(rad);
    GetGameObject().lock()->SetPosition(pos);

    //番号
    enemynumber = GenerateEnemyNumber();

    //バランス調整項目を設定
    spd = MakeSpeed();
    life = MakeLife();
    fire.spread = MakeFirespread();
    fire.attack = MakeFireattack();
    fire.speed = MakeFirespeed();
    fire.life = MakeFirelife();
}

namespace DataName
{
    constexpr nByte Damage = 1;
    constexpr nByte Target = 2;
    constexpr nByte EnemyNumber = 3;
    constexpr nByte ServerTime = 99;
};  // namespace DataName

void Enemy::SetTarget()
{
    const auto len = playerManager->players.size();

    int playerNr = 0;

    //防衛オブジェクトを標的に設定
    fire.targetplayer = TargetObject;
    double distance = GetGameObject().lock()->GetPosition().distanceFrom(TargetObject->GetPosition());

    for (int i = 0; i < len; ++i)
    {
        const auto p = playerManager->players[i]->GetGameObject().lock();
        const auto pl = p->GetComponent<Player>();
        //死んでたらスキップ
        if (pl->GetState() == Player::PlayerStates::reviving)
        {
            continue;
        }

        const auto d = GetGameObject().lock()->GetPosition().distanceFrom(p->GetPosition());
        //最短を更新
        if (d < distance)
        {
            playerNr = pl->playerNr;
            fire.targetplayer = p;
            distance = d;
        }
    }

    const auto range = fire.life * fire.speed * 1.2;
    //射程内に敵がいなければスキップ
    if (range < distance)
    {
        //毎フレームだとコストが大きいので0.1秒に1回確認
        fire.reloadtime = 0.1;
        return;
    }

    //ターゲットプレイヤー番号を送信
    SendSyncInfo(0, playerNr);

    //再初期化
    Reload();
}

//発射間隔などを再設定
void Enemy::Reload()
{
    fire.interval = 0.5;
    fire.rapidnum = 3;
    fire.reloadtime = 3.3;
}

void Enemy::Start2() {}

void Enemy::Update()
{
    Move();
    Shot();
}

void Enemy::Shot()
{
    fire.interval -= s3d::Scene::DeltaTime();
    fire.reloadtime -= s3d::Scene::DeltaTime();

    //自分自身かつ、再ターゲットが可能になっていたら実行
    if (fire.reloadtime < 0 && isMine)
    {
        SetTarget();
    }

    //連射回数が切れていたらなにもしない
    if (fire.rapidnum <= 0)
    {
        return;
    }

    if (fire.interval < 0)
    {
        fire.interval = 0.5;
        fire.rapidnum--;

        //攻撃ベクトルを計算
        auto target = fire.targetplayer->GetPosition();
        target = target - GetGameObject().lock()->GetPosition();

        enemyManager->CreateBullet(*this, target, fire.speed, fire.life, fire.attack);

        if (3 <= fire.spread)
        {
            enemyManager->CreateBullet(*this, target.rotated(Utilities::DegToRad(20)), fire.speed, fire.life, fire.attack);
            enemyManager->CreateBullet(*this, target.rotated(Utilities::DegToRad(-20)), fire.speed, fire.life, fire.attack);
        }

        if (5 <= fire.spread)
        {
            enemyManager->CreateBullet(*this, target.rotated(Utilities::DegToRad(40)), fire.speed, fire.life, fire.attack);
            enemyManager->CreateBullet(*this, target.rotated(Utilities::DegToRad(-40)), fire.speed, fire.life, fire.attack);
        }
    }
}

void Enemy::Move()
{
    auto diff = TargetObject->GetPosition() - GetGameObject().lock()->GetPosition();

    const auto range = fire.life * fire.speed;
    //接近の閾値以下なら移動はしない
    if (diff.length() < range * 0.5)
    {
        return;
    }

    auto g = GetGameObject().lock();
    const auto p = g->GetPosition() + diff.normalized() * spd * s3d::Scene::DeltaTime();
    g->SetPosition(p);
}

//敵ターゲットの設定とHPの同期は同時に行う
//敵ターゲット変更したいだけの時はdamage = 0にすればおっけー
//うーんあとで作り直すかも
void Enemy::SendSyncInfo(int damage, int targetPlayer) const
{
    //現在位置
    //    const s3d::Vec2 pos = GetGameObject().lock()->GetLocalPosition();
    //    const int servertime = networkSystem->GetServerTime();

    // ExitGames::Common::Hashtable table;
    ExitGames::Common::Dictionary<nByte, int> dic;

    // 現在座標
    // 座標は毎回送る必要なし
    // dic.put(0, static_cast<int>(pos.x));
    // dic.put(1, static_cast<int>(pos.y));

    //受けたダメージ
    dic.put(DataName::Damage, damage);

    //ターゲット変更に関する情報
    //負の数は処理しないとき
    dic.put(DataName::Target, targetPlayer);

    //番号
    dic.put(DataName::EnemyNumber, enemynumber);

    //現在時刻
    dic.put(DataName::ServerTime, networkSystem->GetServerTime());

    networkSystem->GetClient().opRaiseEvent(true, dic, CustomEvent::EnemySync);
}

void Enemy::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    if (eventCode != CustomEvent::EnemySync)
    {
        return;
    }

    ExitGames::Common::Dictionary<nByte, int> dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int>>(eventContent).getDataCopy();

    const int nr = *dic.getValue(DataName::EnemyNumber);

    //自分宛のメッセージじゃなければスルー
    if (nr != enemynumber)
    {
        return;
    }

    const int damage = *dic.getValue(DataName::Damage);
    Damage(damage);

    const int target = *dic.getValue(DataName::Target);
    // 0以上なら、ターゲットの設定を行う
    if (target < 0)
    {
        return;
    }

    // 0がオブジェクトコアだが、これでは複数ターゲットに対応できないので要改善
    if (target == 0)
    {
        fire.targetplayer = TargetObject;
    }
    else
    {
        const auto len = playerManager->players.size();
        for (int i = 0; i < len; ++i)
        {
            const auto& player = playerManager->players[i];
            if (player->playerNr == target)
            {
                fire.targetplayer = player->GetGameObject().lock();
                break;
            }
        }
    }

    Reload();
}

void Enemy::Damage(int damage)
{
    life -= damage;

    if (life <= 0)
    {
        // TODO: 消滅エフェクト
        GetGameObject().lock()->GetScene().lock()->Destroy(this->GetGameObject().lock());
    }
}

void Enemy::OnStayCollision(std::shared_ptr<GameObject>& obj)
{
    //弾当たったら
    if (obj->GetTag() == UserDef::Tag::PlayerBullet)
    {
        auto bullet = obj->GetComponent<Bullet>();

        //自分のプレイヤーが発射した弾でなければ何もしない
        if (!bullet->isMine)
        {
            return;
        }

        Damage(bullet->attack);
        SendSyncInfo(bullet->attack);
    }
}

//受信データを元に数値を設定
int Enemy::SetDataFromDictionary(ExitGames::Common::Dictionary<nByte, int>& dic)
{
    const auto posx = *dic.getValue(DataName::Enemy::posX);
    const auto posy = *dic.getValue(DataName::Enemy::posY);
    enemynumber = *dic.getValue(DataName::Enemy::Number);
    spd = *dic.getValue(DataName::Enemy::MoveSpeed);
    life = *dic.getValue(DataName::Enemy::Life);

    fire.spread = *dic.getValue(DataName::Enemy::BulletSpread);
    fire.attack = *dic.getValue(DataName::Enemy::BulletAttack);
    fire.speed = *dic.getValue(DataName::Enemy::BulletSpeed);
    fire.life = *dic.getValue(DataName::Enemy::BulletLife);

    const int time = *dic.getValue(DataName::Enemy::ServerTime);

    GetGameObject().lock()->SetPosition({posx, posy});

    // servertimeを元にposをずらす

    return life;
}

//辞書データを作成
std::unique_ptr<ExitGames::Common::Dictionary<nByte, int>> Enemy::GenerateAndGetData()
{
    //ステータスを生成
    GenerateStatus();

    //それを辞書データにして返す
    auto dic = std::make_unique<ExitGames::Common::Dictionary<nByte, int>>();

    const auto obj = GetGameObject().lock();
    const auto pos = obj->GetPosition();

    dic->put(DataName::Enemy::Type, EnemyType::Standard);

    dic->put(DataName::Enemy::posX, pos.x);
    dic->put(DataName::Enemy::posY, pos.y);
    dic->put(DataName::Enemy::Number, enemynumber);
    dic->put(DataName::Enemy::MoveSpeed, spd);
    dic->put(DataName::Enemy::Life, life);

    dic->put(DataName::Enemy::BulletSpread, fire.spread);
    dic->put(DataName::Enemy::BulletAttack, fire.attack);
    dic->put(DataName::Enemy::BulletSpeed, fire.speed);
    dic->put(DataName::Enemy::BulletLife, fire.life);
    // dic->put(DataName::Enemy::Number, enemynumber);

    dic->put(DataName::Enemy::ServerTime, ComponentEngine::Photon::NetworkSystem::GetInstance()->GetServerTime());
    return dic;
}

EnemyManager* Enemy::enemyManager;
PlayerManager* Enemy::playerManager;

//プレイヤーの基礎火力は攻撃力10の弾を秒間4発なので、それを考慮して生成すること

constexpr int speed_basevalue[] = {35, 45, 47, 51, 55};

int MakeSpeed()
{
    const int difficulty = Matching::GetDifficulty();

    int ret = speed_basevalue[difficulty];

    return ret;
}

int MakeLife()
{
    const int difficulty = Matching::GetDifficulty();

    constexpr int basevalue[] = {20, 30, 40, 40, 20};
    int ret = basevalue[difficulty];

    return ret;
}

int MakeFirespread()
{
    const int difficulty = Matching::GetDifficulty();
    int ret = 1;
    if (static_cast<int>(Difficult::HARD) <= difficulty)
    {
        ret = 3;
    }
    if (static_cast<int>(Difficult::EXTREME) <= difficulty)
    {
        ret = 5;
    }

    return ret;
}

int MakeFireattack()
{
    const int difficulty = Matching::GetDifficulty();

    constexpr int basevalue[] = {10, 12, 15, 20, 32};
    int ret = basevalue[difficulty];

    return ret;
}

int MakeFirespeed()
{
    const int difficulty = Matching::GetDifficulty();

    int ret = speed_basevalue[difficulty] * 2;

    return ret;
}

int MakeFirelife()
{
    const int difficulty = Matching::GetDifficulty();

    int ret = 2.5;

    return ret;
}
