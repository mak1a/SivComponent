
#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"
#include "../../../Utilities/IntervalCall.hpp"

class UIManager;

class PlayerCore : public Photon::AttachableComponentPhotonCallbacks
{
    int maxlife = 1000;
    int life;
    int framethickness = 5;
    int lastsynclife = -1;
    std::shared_ptr<Siv::RectFrame> frame;
    std::shared_ptr<Siv::Rect> rect;

    Utilities::IntervalCall hpSync;

    void Start2() override;
    void Update() override;
    void OnStayCollision(std::shared_ptr<GameObject>& other) override;
    void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

    void PlayerCoreSync();
    void OnDefeat();

public:
    PlayerCore()
        : hpSync(1000 / 3, [&]() { PlayerCoreSync(); })
    {
    }

    int GetMaxLife() const
    {
        return maxlife;
    }
    void SetLife(int _life);
    int GetLife() const;

    std::shared_ptr<UIManager> uimanager;
};
