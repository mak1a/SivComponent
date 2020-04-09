
#include "PlayerCustomGUIs.hpp"
#include "../Common/CommonMemory.hpp"

void PlayerTypeSave::Update()
{
    CommonMemory::SetPlayerType(static_cast<int>(radio->GetIndex()));
}
