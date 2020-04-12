
#include "ReturnTitleBt.hpp"

void ReturnTitleBt::Start2()
{
    GetGameObject().lock()->GetComponent<Siv::Button>()->SetDelegate([&]() { this->OnClick(); });
}

void ReturnTitleBt::OnClick()
{
    //切断
    networkSystem->Disconnect();

    s3d::AudioAsset(U"OK").playOneShot(0.3);

    //シーン変更
    GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
}