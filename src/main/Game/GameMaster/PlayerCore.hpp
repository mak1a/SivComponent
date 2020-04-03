
#include "../../../ComponentEngine/ComponentEngine.hpp"
#include "../../../PhotonComponent/PhotonComponent.hpp"

class UIManager;

class PlayerCore : public Photon::AttachableComponentPhotonCallbacks
{
    int life = 10000;
    int framethickness = 5;
    std::shared_ptr<Siv::RectFrame> frame;
    std::shared_ptr<Siv::Rect> rect;

    void Start2() override;
    void Update() override;
    void OnStayCollision(std::shared_ptr<GameObject>& other) override;

public:
    void SetLife(int _life);
    int GetLife() const;

    std::shared_ptr<UIManager> uimanager;
};