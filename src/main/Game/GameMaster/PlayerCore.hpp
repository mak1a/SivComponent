
#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class UIManager;

class PlayerCore : public Photon::AttachableComponentPhotonCallbacks
{
    const int maxlife = 1000;
    int life;
    int framethickness = 5;
    std::shared_ptr<Siv::RectFrame> frame;
    std::shared_ptr<Siv::Rect> rect;

    void Start2() override;
    void Update() override;
    void OnStayCollision(std::shared_ptr<GameObject>& other) override;

public:
    int GetMaxLife() const
    {
        return maxlife;
    }
    void SetLife(int _life);
    int GetLife() const;

    std::shared_ptr<UIManager> uimanager;
};
