#include "NetworkTest/ITTitlePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkTest/ITLobbyGameMode.h"
#include "NetworkTest/ITTitleGameMode.h"

void AITTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController()) return;
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
}

void AITTitlePlayerController::JoinServer(const FString& AddressOrMap)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*AddressOrMap), true);
}

void AITTitlePlayerController::ServerRPC_StartMatchmaking_Implementation()
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

void AITTitlePlayerController::ServerRPC_JoinMatchmakingQueue_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerRPC_JoinMatchmakingQueue_Implementation called on server"));
	UE_LOG(LogTemp, Warning, TEXT("PlayerController: %s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("HasAuthority: %s"), HasAuthority() ? TEXT("True") : TEXT("False"));

	// ���� GameMode Ȯ��
	AGameModeBase* CurrentGameMode = GetWorld()->GetAuthGameMode();
	if (!CurrentGameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get current GameMode"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Current GameMode: %s"), *CurrentGameMode->GetClass()->GetName());

	// Title �ʿ����� �ٸ� ������ �̵��϶�� �޽����� ���
	if (AITTitleGameMode* TitleGameMode = Cast<AITTitleGameMode>(CurrentGameMode))
	{
		UE_LOG(LogTemp, Warning, TEXT("This is Title GameMode. Need to travel to Lobby first!"));
		// Ŭ���̾�Ʈ���� �˸��� �����ų�, ���� IT_TestEntry�� �̵�
		//ClientRPC_ShowMessage(TEXT("Please connect to Lobby first!"));
		return;
	}

	// LobbyGameMode�� ��� ��Ī ��⿭�� �߰�
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

void AITTitlePlayerController::ServerRPC_LeaveMatchmakingQueue_Implementation()
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