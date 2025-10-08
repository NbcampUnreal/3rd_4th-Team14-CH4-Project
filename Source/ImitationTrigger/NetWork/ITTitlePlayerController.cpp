#include "Network/ITTitlePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Network/ITLobbyGameMode.h"
#include "Network/ITTitleGameMode.h"
#include "Network/ITTitleWidget.h"
#include "Network/ITLobbyWidget.h"

void AITTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		//현재 맵 이름 확인
		FString CurrentMapName = GetWorld()->GetMapName();

		// Title/로비 맵에서는 UI 모드, 게임 맵에서는 게임 모드
		if (CurrentMapName.Contains(TEXT("Title")) ||
			CurrentMapName.Contains(TEXT("Lobby")) ||
			CurrentMapName.Contains(TEXT("Entry")))
		{
			SetupUI();  // UI 모드 설정
		}
		else
		{
			SetGameInputMode();  // 게임 모드 설정
		}
	}
}

void AITTitlePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 게임 종료 시 매칭 큐에서 자동 제거
	if (bIsInMatchmakingQueue && EndPlayReason != EEndPlayReason::LevelTransition)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player disconnecting - Remove from matchmaking queue"));
		ServerRPC_LeaveMatchmakingQueue();
	}

	Super::EndPlay(EndPlayReason);
}

void AITTitlePlayerController::SetGameInputMode()
{
	// 포트나이트 스타일 입력 모드
	FInputModeGameAndUI GameAndUIMode;
	GameAndUIMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	GameAndUIMode.SetHideCursorDuringCapture(true);
	SetInputMode(GameAndUIMode);

	bShowMouseCursor = false;
	UE_LOG(LogTemp, Warning, TEXT("Game input mode activated"));
}

void AITTitlePlayerController::SetupUI()
{
	FString CurrentMapName = GetWorld()->GetMapName();

	if (CurrentMapName.Contains(TEXT("Title")))
	{
		// 타이틀 맵: 타이틀 위젯 생성
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
	else if (CurrentMapName.Contains(TEXT("Lobby")) || CurrentMapName.Contains(TEXT("Entry")))
	{
		// 로비 맵: 로비 위젯 생성
		if (LobbyUIClass)
		{
			LobbyUIInstance = CreateWidget<UUserWidget>(this, LobbyUIClass);
			if (LobbyUIInstance)
			{
				LobbyUIInstance->AddToViewport();
				FInputModeUIOnly Mode;
				Mode.SetWidgetToFocus(LobbyUIInstance->GetCachedWidget());
				SetInputMode(Mode);
				bShowMouseCursor = true;
			}
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
			bIsInMatchmakingQueue = true;
			ClientRPC_UpdateMatchmakingState(true);

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

			// 서버에서 상태 업데이트
			bIsInMatchmakingQueue = false;
			ClientRPC_UpdateMatchmakingState(false);

			UE_LOG(LogTemp, Warning, TEXT("Successfully left matchmaking queue"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get GameMode in ServerRPC_LeaveMatchmakingQueue"));
		}
	}
}

void AITTitlePlayerController::ClientRPC_UpdateMatchmakingState_Implementation(bool bIsInQueue)
{
	// 클라이언트 상태 동기화
	bIsInMatchmakingQueue = bIsInQueue;

	// 로비 위젯에만 매칭 상태 업데이트 (타이틀 위젯은 매칭 기능 없음)
	if (UITLobbyWidget* LobbyWidget = Cast<UITLobbyWidget>(LobbyUIInstance))
	{
		LobbyWidget->UpdateMatchmakingState(bIsInQueue);
	}

	UE_LOG(LogTemp, Warning, TEXT("Client matchmaking state updated: %s"),
		bIsInQueue ? TEXT("In Queue") : TEXT("Not In Queue"));
}
