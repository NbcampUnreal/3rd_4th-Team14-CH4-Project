#pragma once
#include "CoreMinimal.h"
#include "ITMatchTypes.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None,
	WaitingForPlayers,    // �÷��̾� ���
	InMatchmaking,        // ��Ī ���� ��
	CountingDown,         // ���� ���� ī��Ʈ�ٿ�
	Playing,              // ���� ���� ��
	Ending,
	End
};

// ��Ī ��⿭ �÷��̾� ����
USTRUCT(BlueprintType)
struct FMatchmakingPlayer
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	float JoinTime;  // ��⿭ ���� �ð�

	FMatchmakingPlayer()
	{
		PlayerController = nullptr;
		PlayerName = TEXT("");
		JoinTime = 0.0f;
	}
};