#include "Network/ITLobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Network/ITTitlePlayerController.h"
#include "ITGameInstance.h"

void UITLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 로비 기능 버튼들 바인딩
	if (StartButton && !StartButton->OnClicked.IsAlreadyBound(this, &ThisClass::OnStartClicked))
		StartButton->OnClicked.AddDynamic(this, &ThisClass::OnStartClicked);

	if (ExitButton && !ExitButton->OnClicked.IsAlreadyBound(this, &ThisClass::OnExitClicked))
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitClicked);

	if (Character0Button && !Character0Button->OnClicked.IsAlreadyBound(this, &ThisClass::OnCharacter0Clicked))
		Character0Button->OnClicked.AddDynamic(this, &ThisClass::OnCharacter0Clicked);

	if (Character1Button && !Character1Button->OnClicked.IsAlreadyBound(this, &ThisClass::OnCharacter1Clicked))
		Character1Button->OnClicked.AddDynamic(this, &ThisClass::OnCharacter1Clicked);

	if (Character2Button && !Character2Button->OnClicked.IsAlreadyBound(this, &ThisClass::OnCharacter2Clicked))
		Character2Button->OnClicked.AddDynamic(this, &ThisClass::OnCharacter2Clicked);

	// 초기 버튼 모양 설정
	UpdateStartButtonAppearance();
}

void UITLobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 1초마다 매칭 상태 확인
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

void UITLobbyWidget::OnStartClicked()
{
	if (AITTitlePlayerController* PC = GetOwningPlayer<AITTitlePlayerController>())
	{
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
		}
	}
}

void UITLobbyWidget::OnExitClicked()
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

void UITLobbyWidget::UpdateMatchmakingState(bool bIsInQueue)
{
	bIsInMatchmakingQueue = bIsInQueue;
	UpdateStartButtonAppearance();

	UE_LOG(LogTemp, Warning, TEXT("Lobby matchmaking state updated: %s"),
		bIsInQueue ? TEXT("In Queue") : TEXT("Not In Queue"));
}

void UITLobbyWidget::OnCharacter0Clicked()
{
	SelectedCharacterIndex = 0;

	if (UITGameInstance* GI = Cast<UITGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GI->SetSelectedCharacterIndex(0);
		UE_LOG(LogTemp, Warning, TEXT("Character 0 selected"));
	}

}

void UITLobbyWidget::OnCharacter1Clicked()
{
	SelectedCharacterIndex = 1;

	if (UITGameInstance* GI = Cast<UITGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GI->SetSelectedCharacterIndex(1);
		UE_LOG(LogTemp, Warning, TEXT("Character 1 selected"));
	}

}

void UITLobbyWidget::OnCharacter2Clicked()
{
	SelectedCharacterIndex = 2;

	if (UITGameInstance* GI = Cast<UITGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GI->SetSelectedCharacterIndex(2);
		UE_LOG(LogTemp, Warning, TEXT("Character 2 selected"));
	}

}

void UITLobbyWidget::UpdateStartButtonAppearance()
{
	if (StartButtonText && StartButton)
	{
		if (bIsInMatchmakingQueue)
		{
			// 매칭 중: 빨간색 "Match Cancel"
			StartButtonText->SetText(FText::FromString(TEXT("게임 찾는 중(취소)")));
			//StartButton->SetBackgroundColor(FLinearColor::Red);
		}
		else
		{
			// 대기 중: 초록색 "Match Start"
			StartButtonText->SetText(FText::FromString(TEXT("게임 시작")));
			//StartButton->SetBackgroundColor(FLinearColor::Green);
		}
	}
}
