#define NO_S3D_USING
#include "PhotonInitScene.hpp"

namespace PhotonComponent
{
    //シーン変更するだけのクラス
    class SceneChanger : public ComponentEngine::AttachableComponent
    {
        void Update() override
        {
            gameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
        }
    };

    void PhotonInitScene::Setup()
    {
        auto photon = GetSceneManager()->CreateAndGetCommonObject("PhotonSystem");
        photon->AddComponent<NetworkSystem>();

        CreateAndGetObject()->AddComponent<SceneChanger>();
    };


}  // namespace PhotonComponent
