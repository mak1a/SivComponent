
#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class PlayerCore : public Photon::AttachableComponentPhotonCallbacks
{
    int life = 1000;

    void OnStayCollision(std::shared_ptr<GameObject>& other) override;

public:
    void SetLife(int _life);
    int GetLife() const;
};