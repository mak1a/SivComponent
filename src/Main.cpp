
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "ComponentEngine/ComponentEngine.hpp"

using std::cout;
using std::endl;

class TestComponent : public ComponentEngine::AttachableComponent
{
    int x;

public:

    virtual std::string ClassName(){return "TestComponent";}

    TestComponent(){
        x = 2;
    }

    void Start() override
    {
        cout << ClassName() << " Start()" << endl;
        x = 5;
    }

    void Update() override
    {
        if(x > 0)
        {
            cout << ClassName() << " Update()" << x-- << endl;
        }
    }

    virtual ~TestComponent()
    {
        cout << "~TestComponent()" << endl;
    }
};

class SampleComponent : public ComponentEngine::AttachableComponent
{
    void Start() override
    {
        cout << "Sample" << " Start()" << endl;
    }
};

class OverrideComponent : public TestComponent
{
    std::string ClassName() override {return "OverrideComponent";}
};

void Main()
{
    //シーン生成
    ComponentEngine::Scene scene;

    //ゲームオブジェクト生成
    auto object = ComponentEngine::CreateGameObject();
    object->transform().name = "object";

    //コンポーネントを追加
    object->AddComponent<TestComponent>();


    auto obj2 = ComponentEngine::CreateGameObject();
    obj2->transform().name = "obj2";
    obj2->AddComponent<TestComponent>();

    //シーンにオブジェクトを追加
    scene.AddObject(object);
//    scene.AddObject(obj2);
    //子オブジェクトにする
    object->AddChild(obj2);

    std::cout << "Engine Start" << std::endl;

    //StartとUpdateが呼ばれる
    scene.Update();

    std::cout << "\nAdd Component Event" << std::endl;
    //途中でコンポーネントを追加
    obj2->AddComponent<SampleComponent>();
    obj2->AddComponent<OverrideComponent>();
    object->DeleteChild(obj2);

    int k = 5;
    while (System::Update())
    {
        scene.Update();
        std::cout << "" << std::endl;
        if(k-- < 0)
        {
            break;
        }
    }
}

//
// = アドバイス =
// macOS 10.15 Catalina で、アプリケーションを起動するたびに
// ファイルアクセス許可のダイアログが表示される場合、プロジェクトのフォルダを
// User/アプリケーション に移動させることで通常は表示されなくなります。
// 特別なファイルシステム関数の使用や、Web カメラ、マイク使用時のダイアログは消せません。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D Slack (ユーザコミュニティ) への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
