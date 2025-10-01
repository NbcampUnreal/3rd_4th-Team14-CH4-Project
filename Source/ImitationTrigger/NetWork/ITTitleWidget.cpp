#include "Network/ITTitleWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Network/ITTitlePlayerController.h"
#include "Network/ITLobbyGameMode.h"

void UITTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (PlayButton && !PlayButton->OnClicked.IsAlreadyBound(this, &ThisClass::OnPlayClicked))
		PlayButton->OnClicked.AddDynamic(this, &ThisClass::OnPlayClicked);
	if (ExitButton && !ExitButton->OnClicked.IsAlreadyBound(this, &ThisClass::OnExitClicked))
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitClicked);
	if (StartButton && !StartButton->OnClicked.IsAlreadyBound(this, &ThisClass::OnStartClicked))
		StartButton->OnClicked.AddDynamic(this, &ThisClass::OnStartClicked);
}

void UITTitleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 1초마다 상태 동기화 확인
	static float CheckTimer = 0.0f;
	CheckTimer += InDeltaTime;

	if (CheckTimer >= 1.0f)
	{
		CheckTimer = 0.0f;

		if (AITTitlePlayerController* PC = GetOwningPlayer<AITTitlePlayerController>())
		{
			bool bCurrentState = PC->IsInMatchmakingQueue();
			if (bCurrentState != bIsInMatchmakingQueue)
			{
				UpdateMatchmakingState(bCurrentState);
			}
		}
	}
}

void UITTitleWidget::OnPlayClicked()
{
	// 서버 접속 버튼
	if (AITTitlePlayerController* PC = GetOwningPlayer<AITTitlePlayerController>())
	{
		const FString Address = ServerIPEdit ? ServerIPEdit->GetText().ToString() : TEXT("1.224.158.94:7777");
		UE_LOG(LogTemp, Warning, TEXT("Trying to connect to: %s"), *Address);
		PC->JoinServer(Address);
	}
}

void UITTitleWidget::OnExitClicked()
{
	// 게임 종료 전에 매칭 큐에서 제거
	if (bIsInMatchmakingQueue)
	{
		if (AITTitlePlayerController* PC = GetOwningPlayer<AITTitlePlayerController>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Leaving matchmaking queue before exit"));
			PC->ServerRPC_LeaveMatchmakingQueue();
		}
	}
	// 게임 종료
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UITTitleWidget::OnStartClicked()
{
	if (AITTitlePlayerController* PC = GetOwningPlayer<AITTitlePlayerController>())
	{
		// 네트워크 상태 확인
		UE_LOG(LogTemp, Warning, TEXT("=== Start Button Debug Info ==="));
		UE_LOG(LogTemp, Warning, TEXT("PlayerController: %s"), PC ? TEXT("Valid") : TEXT("NULL"));
		UE_LOG(LogTemp, Warning, TEXT("IsLocalController: %s"), PC->IsLocalController() ? TEXT("True") : TEXT("False"));
		UE_LOG(LogTemp, Warning, TEXT("NetMode: %d"), GetWorld()->GetNetMode());
		UE_LOG(LogTemp, Warning, TEXT("HasAuthority: %s"), PC->HasAuthority() ? TEXT("True") : TEXT("False"));

		if (bIsInMatchmakingQueue)
		{
			// 매칭 취소
			UE_LOG(LogTemp, Warning, TEXT("Cancelling matchmaking"));
			PC->ServerRPC_LeaveMatchmakingQueue();
		}
		else
		{
			// 매칭 시작
			UE_LOG(LogTemp, Warning, TEXT("Starting matchmaking"));
			PC->ServerRPC_JoinMatchmakingQueue();
			UE_LOG(LogTemp, Warning, TEXT("ServerRPC_JoinMatchmakingQueue called"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
	}
}

void UITTitleWidget::UpdateMatchmakingState(bool bIsInQueue)
{
	bIsInMatchmakingQueue = bIsInQueue;
	UpdateStartButtonAppearance();

	UE_LOG(LogTemp, Warning, TEXT("Matchmaking state updated: %s"),
		bIsInQueue ? TEXT("In Queue") : TEXT("Not In Queue"));
}

void UITTitleWidget::UpdateStartButtonAppearance()
{
	if (StartButtonText)
	{
		if (bIsInMatchmakingQueue)
		{
			// 매칭 중: 빨간색 "Match Cancel"
			StartButtonText->SetText(FText::FromString(TEXT("Match Cancel")));
			StartButton->SetBackgroundColor(FLinearColor::Red);
		}
		else
		{
			// 대기 중: 초록색 "Match Start"
			StartButtonText->SetText(FText::FromString(TEXT("Match Start")));
			StartButton->SetBackgroundColor(FLinearColor::Green);
		}
	}
}
