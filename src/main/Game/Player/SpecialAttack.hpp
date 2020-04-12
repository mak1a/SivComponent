
#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class PlayerAnimation;
class Player;
class ISpecialAttack : public Photon::AttachableComponentPhotonCallbacks
{
protected:
    bool hassp = true;
    std::shared_ptr<Player> player;
    std::shared_ptr<PlayerAnimation> playerAnime;
    // std::shared_ptr<Player> player;
    virtual void OnSpecial() = 0;
    //スペシャルのリモート同期
    virtual void OnRemoteSpecial(const ExitGames::Common::Hashtable&) = 0;

    virtual void Start3(){};
    void Start2() override;

    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

public:
    void Special();
};

class EnemyManager;
class SpyBomb : public ISpecialAttack
{
    enum DataName
    {
        Damage
    };

    std::shared_ptr<EnemyManager> eManager;

    void Start3() override;

    void OnSpecial() override;
    void OnRemoteSpecial(const ExitGames::Common::Hashtable& table) override;
};