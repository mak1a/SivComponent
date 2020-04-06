
#define NO_S3D_USING
#include "Title.hpp"
// #include <unicode/schriter.h>
#include "PhotonComponent/NetworkSystem.hpp"
#include "Utilities/TextConvert.hpp"

using namespace ComponentEngine;

class NameGenerator : public ComponentEngine::AttachableComponent
{
public:
    std::weak_ptr<ComponentEngine::Siv::TextBox> tbox;

    struct NamePair
    {
        s3d::String sivstr;
    };

private:
    // s3d::Array<NamePair> nametable;

    static int n;

    void generate_name()
    {
        n = s3d::Random(0, 1000);
    }

public:
    void Awake() override
    {
        if (n == -1)
        {
            generate_name();
        }
    }

    void Start() override
    {
        tbox.lock()->SetText(GetNameSiv());
    }

    s3d::String GetNameSiv()
    {
        return s3d::String(s3d::Format(U"Player", n));
    }

    ExitGames::Common::JString GetNamePhoton()
    {
        return ExitGames::Common::JString(L"Player") + n;
    }
    void Generate()
    {
        generate_name();
        tbox.lock()->SetText(GetNameSiv());
    }
};

struct ChangeSceneBt : public ComponentEngine::AttachableComponent
{
    std::shared_ptr<NameGenerator> generator;

    void ChangeToMatching()
    {
        auto manager = GetGameObject().lock()->GetScene().lock()->GetSceneManager();
        manager->GetCommon()
            .GetCommonObject(ComponentEngine::Photon::NetworkObjectName())
            ->GetComponent<ComponentEngine::Photon::NetworkSystem>()
            ->SetPlayerName(generator->GetNamePhoton());
        manager->ChangeScene("Matching");
    }
};

void TitleScene::Setup()
{
    s3d::Scene::SetBackground(s3d::Palette::Skyblue);

    //名前生成
    auto obj = CreateAndGetObject();
    auto genbt = CreateAndGetObject();
    obj->AddChild(genbt);

    obj->SetPosition(s3d::Scene::Center());
    auto gen = genbt->AddComponent<NameGenerator>();
    gen->tbox = obj->AddComponent<ComponentEngine::Siv::TextBox>();
    gen->tbox.lock()->SetIsEnabled(false);

    genbt->AddComponent<ComponentEngine::Siv::Button>()->SetText(U"名前生成").SetDelegate([=]() { gen->Generate(); });
    genbt->SetPosition({0, 40});

    //マッチングボタン
    auto bt = CreateAndGetObject();
    bt->SetPosition({s3d::Scene::Width() / 2, s3d::Scene::Height() / 4 * 3});
    auto btscript = bt->AddComponent<ChangeSceneBt>();
    bt->AddComponent<Siv::Button>()->SetText(U"マッチング開始").SetDelegate([=]() { btscript->ChangeToMatching(); });
    btscript->generator = gen;

    //タイトル
    auto title = CreateAndGetObject();
    title->SetPosition({s3d::Scene::CenterF().x, 100});
    title->AddComponent<Siv::Text>()->SetText(U"オンラインCoopゲーム(仮題 α版)").SetFont(s3d::Font(50)).SetColor(s3d::Palette::Black);
}

int NameGenerator::n = -1;
