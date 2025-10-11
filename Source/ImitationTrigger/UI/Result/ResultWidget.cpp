#include "UI/Result/ResultWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UResultWidget::SetResult(int32 TotalPlayers, int32 PlayerRank, UTexture2D* CharacterImage, FText PlayerName,
                              int32 SurvivalTime, int32 KillCount, float Damage)
{

	if (PlayerRankBlock)
	{
		FString RankText = FString::Printf(TEXT("%d / %d"), PlayerRank, TotalPlayers );
		PlayerRankBlock->SetText(FText::FromString(RankText));
	}

	
	if (PlayerImage)
	{
		FSlateBrush Brush = PlayerImage->GetBrush();
		Brush.SetResourceObject(CharacterImage);
		PlayerImage->SetBrush(Brush);
	}

	if (PlayerNameBlock)
	{
		PlayerNameBlock->SetText(PlayerName);
	}

	if (SurvivalTimeBlock)
	{
		int32 Minutes = SurvivalTime / 60;
		int32 Seconds = SurvivalTime % 60;

		FString Time = FString::Printf(TEXT("%d:%02d"), Minutes, Seconds);
		SurvivalTimeBlock->SetText(FText::FromString(Time));
	}

	if (KillCountBlock)
	{
		FText KillNumber = FText::AsNumber(KillCount);
		KillCountBlock->SetText(KillNumber);
	}

	if (DamageBlock)
	{
		FText DamageNumber = FText::AsNumber(Damage);
		DamageBlock->SetText(DamageNumber);
	}
	
}

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 이벤트 바인딩
	if (ReturnToLobbyButton)
	{
		ReturnToLobbyButton->OnClicked.AddDynamic(this, &UResultWidget::OnReturnToLobbyClicked);
	}
}

void UResultWidget::SetWinnerName(const FString& WinnerName)
{
	if (WinnerText)
	{
		FString ResultText = FString::Printf(TEXT("승자: %s"), *WinnerName);
		WinnerText->SetText(FText::FromString(ResultText));
	}
}

void UResultWidget::OnReturnToLobbyClicked()
{
#if WITH_EDITOR
	// 에디터 환경: ConsoleCommand로 Open 실행
	const FString LobbyMapName = TEXT("IT_TestEntry");
	UE_LOG(LogTemp, Warning, TEXT("EDITOR MODE: Opening lobby map - %s"), *LobbyMapName);

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->ConsoleCommand(FString::Printf(TEXT("Open %s"), *LobbyMapName));
	}
#else
	// 패키징 환경: ClientTravel로 로비 서버 연결
	FString LobbyAddress = GetLobbyServerAddress();

	if (!LobbyAddress.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Returning to lobby: %s"), *LobbyAddress);

		if (APlayerController* PC = GetOwningPlayer())
		{
			PC->ClientTravel(LobbyAddress, TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get lobby server address"));
	}
#endif
}

FString UResultWidget::GetLobbyServerAddress()
{
#if WITH_EDITOR
	// 에디터에서는 이 함수가 호출되지 않음
	return FString();
#else
	UWorld* World = GetWorld();
	if (!World) return FString();

	FString CurrentURL = World->URL.ToString();
	UE_LOG(LogTemp, Log, TEXT("Current URL: %s"), *CurrentURL);

	// URL 파싱
	FString AddressPart;
	int32 QueryIndex = CurrentURL.Find(TEXT("?"));
	if (QueryIndex != INDEX_NONE)
	{
		AddressPart = CurrentURL.Left(QueryIndex);
	}
	else
	{
		AddressPart = CurrentURL;
	}

	// 맵 이름 제거
	int32 LastSlashIndex;
	if (AddressPart.FindLastChar('/', LastSlashIndex))
	{
		AddressPart = AddressPart.Mid(LastSlashIndex + 1);
	}

	// IP:Port 형식 확인
	int32 ColonIndex;
	if (AddressPart.FindChar(':', ColonIndex))
	{
		FString IPAddress = AddressPart.Left(ColonIndex);
		// 7777 포트의 로비 서버로 연결
		FString LobbyAddress = FString::Printf(TEXT("%s:7777"), *IPAddress);
		UE_LOG(LogTemp, Warning, TEXT("Lobby address: %s"), *LobbyAddress);
		return LobbyAddress;
	}

	// NetDriver를 통한 대체 방법
	if (World->GetNetDriver() && World->GetNetDriver()->ServerConnection)
	{
		FString ServerAddress = World->GetNetDriver()->ServerConnection->URL.Host;
		if (!ServerAddress.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Got IP from NetDriver: %s"), *ServerAddress);
			return FString::Printf(TEXT("%s:7777"), *ServerAddress);
		}
	}

	// 기본값
	UE_LOG(LogTemp, Warning, TEXT("Could not extract IP, using localhost"));
	return TEXT("127.0.0.1:7777");
#endif
}
