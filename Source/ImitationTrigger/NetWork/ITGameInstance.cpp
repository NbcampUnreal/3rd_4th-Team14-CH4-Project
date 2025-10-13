#include "NetWork/ITGameInstance.h"

void UITGameInstance::SetPlayerNickname(const FString& Nickname)
{
	PlayerNickname = Nickname.IsEmpty() ? TEXT("Player") : Nickname;
}
