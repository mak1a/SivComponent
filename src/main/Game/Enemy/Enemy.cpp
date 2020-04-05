
#include "Enemy.hpp"
#include "../../../Utilities/Utilities.hpp"
#include "../../CustomEventList.hpp"
#include "../Bullet.hpp"
#include "../Player/Player.hpp"
#include "../Player/PlayerManager.hpp"
#include "EnemyManager.hpp"

namespace DataName
{
    constexpr nByte Damage = 1;
    constexpr nByte Target = 2;
    constexpr nByte EnemyNumber = 3;
    constexpr nByte ServerTime = 99;
};  // namespace DataName

//速度60 * 生存時間4.5　より　距離250以下になったら優先的にコアを狙うようにする
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
        const auto d = GetGameObject().lock()->GetPosition().distanceFrom(p->GetPosition());

        //最短を更新
        if (d < distance)
        {
            playerNr = p->GetComponent<Player>()->playerNr;
            fire.targetplayer = p;
            distance = d;
        }
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

        enemyManager->CreateBullet(*this, target, 60, 4.5);
        enemyManager->CreateBullet(*this, target.rotated(Utilities::DegToRad(20)), 60, 4.5);
        enemyManager->CreateBullet(*this, target.rotated(Utilities::DegToRad(-20)), 60, 4.5);
    }
}

void Enemy::Move()
{
    auto diff = TargetObject->GetPosition() - GetGameObject().lock()->GetPosition();

    //接近の閾値以下なら移動はしない
    if (diff.length() < 150)
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

    ExitGames::Common::Dictionary<nByte, int> dic = ExitGames::Common::ValueObject<ExitGames::Common::Dictionary<nByte, int> >(eventContent).getDataCopy();

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
        const int len = playerManager->players.size();
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
void Enemy::SetDataFromDictionary(ExitGames::Common::Dictionary<nByte, int>*) {}

//辞書データを作成
void Enemy::CreateDataFromDictionary(ExitGames::Common::Dictionary<nByte, int>&) {}

EnemyManager* Enemy::enemyManager;
PlayerManager* Enemy::playerManager;
