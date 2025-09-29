#include "Network/ITTitlePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Network/ITLobbyGameMode.h"
#include "Network/ITTitleGameMode.h"

void AITTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (TitleUIClass)
		{
			TitleUIInstance = CreateWidget<UUserWidget>(this, TitleUIClass);
			if (TitleUIInstance)
			{
				TitleUIInstance->AddToViewport();
				FInputModeUIOnly Mode;
				Mode.SetWidgetToFocus(TitleUIInstance->GetCachedWidget());
				SetInputMode(Mode);
				bShowMouseCursor = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TitleUIClass is null!"));
		}
	}
}

void AITTitlePlayerController::JoinServer(const FString& AddressOrMap)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*AddressOrMap), true);
}

void AITTitlePlayerController::ServerRPC_StartMatchmaking_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerRPC_StartMatchmaking_Implementation called on server"));
		if (AITLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AITLobbyGameMode>())
		{
			GameMode->StartMatchmaking();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get GameMode in ServerRPC_StartMatchmaking"));
		}
	}
}

void AITTitlePlayerController::ServerRPC_JoinMatchmakingQueue_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerRPC_JoinMatchmakingQueue_Implementation called on server"));
		UE_LOG(LogTemp, Warning, TEXT("PlayerController: %s"), *GetName());
		UE_LOG(LogTemp, Warning, TEXT("HasAuthority: %s"), HasAuthority() ? TEXT("True") : TEXT("False"));

		// 현재 GameMode 확인
		AGameModeBase* CurrentGameMode = GetWorld()->GetAuthGameMode();
		if (!CurrentGameMode)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get current GameMode"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Current GameMode: %s"), *CurrentGameMode->GetClass()->GetName());

		// Title 맵에서는 다른 레벨로 이동하라는 메시지만 출력
		if (AITTitleGameMode* TitleGameMode = Cast<AITTitleGameMode>(CurrentGameMode))
		{
			UE_LOG(LogTemp, Warning, TEXT("This is Title GameMode. Need to travel to Lobby first!"));
			// 클라이언트에게 알림을 보내거나, 직접 IT_TestEntry로 이동
			return;
		}

		// LobbyGameMode인 경우 매칭 대기열에 추가
		if (AITLobbyGameMode* GameMode = Cast<AITLobbyGameMode>(CurrentGameMode))
		{
			GameMode->JoinMatchmakingQueue(this);
			UE_LOG(LogTemp, Warning, TEXT("Successfully joined matchmaking queue"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Current GameMode is not LobbyGameMode: %s"),
				*CurrentGameMode->GetClass()->GetName());
		}
	}
}

void AITTitlePlayerController::ServerRPC_LeaveMatchmakingQueue_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerRPC_LeaveMatchmakingQueue_Implementation called on server"));
		if (AITLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AITLobbyGameMode>())
		{
			GameMode->LeaveMatchmakingQueue(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get GameMode in ServerRPC_LeaveMatchmakingQueue"));
		}
	}
}
