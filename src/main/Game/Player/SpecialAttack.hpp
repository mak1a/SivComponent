
#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class PlayerAnimation;
// class Player;
class ISpecialAttack : public Photon::AttachableComponentPhotonCallbacks
{
private:
    bool hassp = true;
    // std::shared_ptr<Player> player;
    std::shared_ptr<PlayerAnimation> playerAnime;
    virtual void OnSpecial() = 0;

    virtual void Start3(){};
    void Start2() override;

public:
    void Special();
};

class EnemyManager;
class SpyBomb : public ISpecialAttack
{
    std::shared_ptr<EnemyManager> eManager;

    void Start3() override;

    void OnSpecial() override;
};