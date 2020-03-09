
#include "TestScene.hpp"

// #include "ComponentEngine/ComponentEngine.hpp"
#include "SivComponent/SivComponent.hpp"

class NextScene : public ComponentEngine::AttachableComponent
{
    void Update() override
    {
        if (KeyA.down())
        {
            gameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Matching");
        }
    }
};

void TestScene::Setup()
{
    //ゲームオブジェクト生成
    auto object = CreateAndGetObject();
    object->SetName("object");
    object->transform().SetPosition({400, 200});
    object->transform().SetRotateByAngle(-15);  //回転させておく

    // Rectコンポーネントを追加
    auto rect = object->AddComponent<ComponentEngine::Siv::Rect>();
    rect->Size = {100, 100};
    //シーンにオブジェクトを追加
    AddObject(object);

    auto obj2 = CreateAndGetObject();
    obj2->SetName("obj2");
    obj2->transform().SetPosition({0, 300});  // y座標だけずらす
    obj2->AddComponent<ComponentEngine::Siv::Text>();
    //子オブジェクトにする
    object->AddChild(obj2);

    //マウス追従オブジェクト
    //共通オブジェクトに設定する
    auto mouseObj = GetSceneManager()->CreateAndGetCommonObject("MouseCircle");
    mouseObj->SetName("mouseObj");
    mouseObj->AddComponent<ComponentEngine::Siv::Circle>()->SetColor(s3d::Palette::Blue).SetR(30);
    mouseObj->AddComponent<ComponentEngine::Siv::MouseChase>();

    CreateAndGetObject()->SetName("SceneChanger").AddComponent<NextScene>();
}