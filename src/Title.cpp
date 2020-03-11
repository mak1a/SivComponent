
#include "Title.hpp"

using namespace ComponentEngine;

struct ChangeSceneBt : public ComponentEngine::AttachableComponent
{
    std::shared_ptr<ComponentEngine::Siv::TextBox> textbox;

    void ChangeToMatching()
    {
        if (textbox->GetText().length() > 0)
        {
            GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Matching");
        }
        else
        {
            //エラー表示
        }
    }
};

void TitleScene::Setup()
{
    s3d::Scene::SetBackground(Palette::Skyblue);

    auto tbox = CreateAndGetObject();
    tbox->transform().SetPosition(s3d::Scene::Center());
    auto boxcom = tbox->AddComponent<ComponentEngine::Siv::TextBox>();
    boxcom->SetMaxLength(20);

    auto bt = CreateAndGetObject();
    bt->transform().SetPosition({s3d::Scene::Width() / 2, s3d::Scene::Height() / 4 * 3});
    auto btscript = bt->AddComponent<ChangeSceneBt>();
    bt->AddComponent<Siv::Button>()->SetText(U"マッチング開始").SetDelegate([=]() { btscript->ChangeToMatching(); });
    btscript->textbox = boxcom;

    auto title = CreateAndGetObject();
    title->transform().SetPosition({0, 100});
    title->AddComponent<Siv::Text>()->SetText(U"オンラインCoopゲーム(仮題 α版)").SetFont(Font(50)).SetColor(Palette::Black);
}
