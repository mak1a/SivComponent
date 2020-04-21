
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
        manager->ChangeScene("Custom");
    }
};

void TitleScene::Setup()
{
    s3d::Scene::SetBackground(s3d::Palette::Lightskyblue);

    //名前生成
    auto obj = CreateAndGetObject();
    auto genbt = CreateAndGetObject();
    obj->AddChild(genbt);

    obj->SetPosition(s3d::Scene::Center().movedBy(0, -80));
    auto gen = genbt->AddComponent<NameGenerator>();
    gen->tbox = obj->AddComponent<ComponentEngine::Siv::TextBox>();
    gen->tbox.lock()->SetIsEnabled(false);

    genbt->AddComponent<ComponentEngine::Siv::Button>()->SetText(U"名前生成").SetDelegate([=]() {
        gen->Generate();
        s3d::AudioAsset(U"OK").playOneShot();
    });
    genbt->SetPosition({0, 60});

    //マッチングボタン
    auto bt = CreateAndGetObject();
    bt->SetPosition({s3d::Scene::Width() / 2, s3d::Scene::Height() / 4 * 2.5});
    auto btscript = bt->AddComponent<ChangeSceneBt>();
    bt->AddComponent<Siv::Button>()->SetText(U"プレイヤー選択へ").SetDelegate([=]() {
        btscript->ChangeToMatching();
        s3d::AudioAsset(U"OK").playOneShot(0.3);
    });
    btscript->generator = gen;

    //タイトル
    auto title = CreateAndGetObject();
    title->SetPosition({s3d::Scene::CenterF().x, 100});
    title->AddComponent<Siv::Text>()->SetText(U"Barrage Storm").SetFont(s3d::Font(65, s3d::Typeface::Heavy)).SetColor(s3d::Palette::Black);
    auto version = title->CreateChild();
    version->SetPosition({s3d::Scene::CenterF().x / 1.5, 0});
    version->AddComponent<Siv::Text>()->SetText(U"v0.2.1").SetFont(s3d::Font(30)).SetColor(s3d::Palette::Black).SetDrawAt(false);

    CreateAndGetObject()->AddComponent<Siv::BGM>(U"BGM_Title");

    auto credit = CreateAndGetObject();
    credit->SetPosition({10, s3d::Scene::Height() - 140});
    credit->AddComponent<Siv::Text>()->SetText(U"魔王魂\n効果音ラボ").SetFont(s3d::Font(30)).SetColor(s3d::Palette::Black).SetDrawAt(false);
}

int NameGenerator::n = -1;
