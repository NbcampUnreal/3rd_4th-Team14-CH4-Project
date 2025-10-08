#include "Network/ITTitleWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Network/ITTitlePlayerController.h"

void UITTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 서버 접속 버튼만 바인딩 (매칭 관련 기능 제거)
	if (JoinServerButton && !JoinServerButton->OnClicked.IsAlreadyBound(this, &ThisClass::OnJoinServerClicked))
		JoinServerButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinServerClicked);
}

void UITTitleWidget::OnJoinServerClicked()
{
	// 서버 접속 기능만 담당
	if (AITTitlePlayerController* PC = GetOwningPlayer<AITTitlePlayerController>())
	{
		const FString Address = ServerIPEdit ? ServerIPEdit->GetText().ToString() : TEXT("127.0.0.1:7777");
		UE_LOG(LogTemp, Warning, TEXT("Connecting to server: %s"), *Address);
		PC->JoinServer(Address);
	}
}
