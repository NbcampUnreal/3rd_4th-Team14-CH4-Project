#include "NetworkTest/ITTitleWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkTest/ITTitlePlayerController.h"
#include "NetworkTest/ITLobbyGameMode.h"

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

void UITTitleWidget::OnPlayClicked()
{
    // 서버 접속 버튼
    if (AITTitlePlayerController* PC = GetOwningPlayer<AITTitlePlayerController>())
    {
        const FString Address = ServerIPEdit ? ServerIPEdit->GetText().ToString() : TEXT("127.0.0.1:7777");
        UE_LOG(LogTemp, Warning, TEXT("Trying to connect to: %s"), *Address);
        PC->JoinServer(Address);
    }
}

void UITTitleWidget::OnExitClicked()
{
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

        // 직접 로컬 GameMode에 접근 시도 (테스트용)
        if (GetWorld()->GetNetMode() == NM_Standalone || GetWorld()->GetNetMode() == NM_ListenServer)
        {
            if (AITLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AITLobbyGameMode>())
            {
                UE_LOG(LogTemp, Warning, TEXT("Direct GameMode access successful! Calling JoinMatchmakingQueue directly"));
                GameMode->JoinMatchmakingQueue(PC);
                return;
            }
        }

        // ServerRPC 호출
        UE_LOG(LogTemp, Warning, TEXT("Calling ServerRPC_JoinMatchmakingQueue"));
        PC->ServerRPC_JoinMatchmakingQueue();
        UE_LOG(LogTemp, Warning, TEXT("ServerRPC_JoinMatchmakingQueue called"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
    }
}