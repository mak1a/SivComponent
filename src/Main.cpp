
#include <Siv3D.hpp>  // OpenSiv3D v0.4.2

#include "ComponentEngine/ComponentEngine.hpp"

using std::cout;
using std::endl;

class TestComponent : public ComponentEngine::AttachableComponent
{
    int x;
    
public:
    
    TestComponent(){
        x = 5;
    }
    
    void Start() override
    {
        cout << "Test Component Start()" << endl;
    }
    
    void Update() override
    {
        if(x > 0)
        {
            cout << "Test Update Update()" << x-- << endl;
        }
    }
};

void Main()
{
    //シーン生成
    ComponentEngine::Scene scene;
    
    //ゲームオブジェクト生成
    auto object =    std::make_shared<ComponentEngine::GameObject>();
    //コンポーネントを追加
    object->AddComponent(std::make_shared<TestComponent>());
    //シーンにオブジェクトを追加
    scene.AddObject(object);
    
    while (System::Update())
    {
        scene.Update();
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
