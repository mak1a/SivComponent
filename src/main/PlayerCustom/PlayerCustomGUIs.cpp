
#include "PlayerCustomGUIs.hpp"
#include "../Common/CommonMemory.hpp"

void PlayerTypeSave::Start()
{
    radio = GetGameObject().lock()->GetComponent<Siv::RadioButton>();
    radio->SetIndex(CommonMemory::GetPlayerType());
}

void PlayerTypeSave::Update()
{
    CommonMemory::SetPlayerType(static_cast<int>(radio->GetIndex()));
}
