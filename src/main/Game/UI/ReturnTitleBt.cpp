
#include "ReturnTitleBt.hpp"

void ReturnTitleBt::Start2()
{
    GetGameObject().lock()->GetComponent<Siv::Button>()->SetDelegate([&]() { this->OnClick(); });
}

void ReturnTitleBt::OnClick()
{
    //切断
    networkSystem->Disconnect();
    //シーン変更
    GetGameObject().lock()->GetScene().lock()->GetSceneManager()->ChangeScene("Title");
}