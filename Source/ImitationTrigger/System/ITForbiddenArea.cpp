#include "System/ITForbiddenArea.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ITCharacter.h"
#include "System/ITLogChannel.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"

AITForbiddenArea::AITForbiddenArea()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	AreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AreaMesh"));

	Round = -1;
	bIsProgressing = 0;
}

void AITForbiddenArea::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Round);
	DOREPLIFETIME(ThisClass, bIsProgressing);
	DOREPLIFETIME(ThisClass, NextCenterPosition);
	DOREPLIFETIME(ThisClass, NextRadiusScale);
}

void AITForbiddenArea::Tick(float DeltaTime)
{
	if (bIsProgressing == 1)
	{
		if (HasAuthority())
		{
			float Alpha = 0.0f;
			if (AreaProgressTimer.IsValid())
			{
				Alpha = GetWorldTimerManager().GetTimerRemaining(AreaProgressTimer) / GetCurrentRoundInfo().ProgressDuration;
			}
			FVector NewCenterPosition = FMath::Lerp(NextCenterPosition, CenterPosition, Alpha);
			float NewRadiusScale = FMath::Lerp(NextRadiusScale, RadiusScale, Alpha);
			UpdateArea(NewCenterPosition, NewRadiusScale);
		}
		else
		{
			float Alpha = ClientSyncInfo.RemainTime / ClientSyncInfo.TotalTime;
			ClientSyncInfo.RemainTime -= DeltaTime;
			if (ClientSyncInfo.RemainTime > 0)
			{
				FVector NewCenterPosition = FMath::Lerp(ClientSyncInfo.NextCenterPosition, ClientSyncInfo.CenterPosition, Alpha);
				float NewRadiusScale = FMath::Lerp(ClientSyncInfo.NextRadiusScale, ClientSyncInfo.RadiusScale, Alpha);
				UpdateArea(NewCenterPosition, NewRadiusScale);
			}
		}
	}
}

const FITForbiddenRoundInfo& AITForbiddenArea::GetCurrentRoundInfo() const
{
	if (RoundInfos.IsValidIndex(Round))
	{
		return RoundInfos[Round];
	}
	else
	{
		return RoundInfos.Last();
	}
}

const FITForbiddenRoundInfo& AITForbiddenArea::GetNextRoundInfo() const
{
	if (RoundInfos.IsValidIndex(Round + 1))
	{
		return RoundInfos[Round + 1];
	}
	else
	{
		return RoundInfos.Last();
	}
}

void AITForbiddenArea::BeginPlay()
{
	Super::BeginPlay();
	
	RadiusScale = GetActorScale3D().X;
	CenterPosition = GetActorLocation();

	OriginRadiusScale = RadiusScale;

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(AreaDamageTimer, this, &ThisClass::OnDamageTimer, 1.0f, true);

		OnRoundStartTimer();
	}
}

void AITForbiddenArea::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (AreaDamageTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AreaDamageTimer);
	}
	if (AreaRoundWaitTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AreaRoundWaitTimer);
	}
	if (AreaProgressTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AreaProgressTimer);
	}
	if (AreaSyncTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AreaSyncTimer);
	}
}


void AITForbiddenArea::OnRep_Round()
{
}

void AITForbiddenArea::OnDamageTimer()
{
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(this, AITCharacter::StaticClass(), Characters);

	for (const AActor* Character : Characters)
	{
		const AITCharacter* ITCharacter = Cast<AITCharacter>(Character);
		if (IsValid(ITCharacter))
		{
			if (!IsInSafeArea(ITCharacter))
			{
				UITAbilitySystemComponent* ITASC = ITCharacter->GetITAbilitySystemComponent();
				ApplyDamage(ITASC);
			}
		}
	}
}

void AITForbiddenArea::OnRoundEndTimer()
{
	IT_LOG_ROLE(LogITNet, Log, TEXT("%d Round End..."), Round);

	const FITForbiddenRoundInfo& CurrentRoundInfo = GetCurrentRoundInfo();
	GetWorldTimerManager().SetTimer(AreaProgressTimer, this, &ThisClass::OnRoundStartTimer, CurrentRoundInfo.ProgressDuration, false);

	//const FITForbiddenRoundInfo& NextRoundInfo = GetNextRoundInfo();
	//NextCenterPosition = GenerateRandomCenterPosition();
	//NextRadiusScale = OriginRadiusScale * NextRoundInfo.AreaPercentage / 100.0f;

	GetWorldTimerManager().SetTimer(AreaSyncTimer, this, &ThisClass::OnSyncTimer, SyncInterval, true);
	OnSyncTimer();
	bIsProgressing = 1;
}

void AITForbiddenArea::OnRoundStartTimer()
{
	if (AreaSyncTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AreaSyncTimer);
	}

	Round += 1;
	bIsProgressing = 0;
	if (Round > 0)
	{
		CenterPosition = NextCenterPosition;
		RadiusScale = NextRadiusScale;
		UpdateArea(CenterPosition, RadiusScale);
	}
	const FITForbiddenRoundInfo& CurrentRoundInfo = GetCurrentRoundInfo();
	GetWorldTimerManager().SetTimer(AreaRoundWaitTimer, this, &ThisClass::OnRoundEndTimer, CurrentRoundInfo.WaitDuration, false);

	const FITForbiddenRoundInfo& NextRoundInfo = GetNextRoundInfo();
	NextCenterPosition = GenerateRandomCenterPosition();
	NextRadiusScale = OriginRadiusScale * NextRoundInfo.AreaPercentage / 100.0f;

	IT_LOG_ROLE(LogITNet, Log, TEXT("%d Round Start"), Round);
}

void AITForbiddenArea::OnSyncTimer()
{
	FITSyncInfo NewSyncInfo;
	NewSyncInfo.CenterPosition = CenterPosition;
	NewSyncInfo.NextCenterPosition = NextCenterPosition;
	NewSyncInfo.RadiusScale = RadiusScale;
	NewSyncInfo.NextRadiusScale = NextRadiusScale;
	NewSyncInfo.TotalTime = GetCurrentRoundInfo().ProgressDuration;
	if (AreaProgressTimer.IsValid())
	{
		NewSyncInfo.RemainTime = GetWorldTimerManager().GetTimerRemaining(AreaProgressTimer);
	}
	else
	{
		NewSyncInfo.RemainTime = 0.0f;
	}
	MulticastRPC_SyncArea(NewSyncInfo);
}

void AITForbiddenArea::MulticastRPC_SyncArea_Implementation(FITSyncInfo SyncInfo)
{
	if (!HasAuthority())
	{
		bIsProgressing = 1;
		ClientSyncInfo = SyncInfo;
	}
}

void AITForbiddenArea::UpdateArea(const FVector& InCenterPosition, float InRadiusScale)
{
	IT_LOG_ROLE(LogITNet, Log, TEXT("InRadiusScale: %f"), InRadiusScale);

	float ZScale = GetActorScale3D().Z;
	FVector NewScale(InRadiusScale, InRadiusScale, ZScale);

	SetActorScale3D(NewScale);
	SetActorLocation(InCenterPosition);
}

FVector AITForbiddenArea::GenerateRandomCenterPosition()
{
	const FITForbiddenRoundInfo& CurrentRoundInfo = GetCurrentRoundInfo();
	const FITForbiddenRoundInfo& NextRoundInfo = GetNextRoundInfo();
	float Radius = GetActorScale().X * 100 / 2;
	float NewRadius = Radius * NextRoundInfo.AreaPercentage / 100.0f;
	float RandRange = Radius - NewRadius;
	float RandomX = FMath::RandRange(CenterPosition.X - RandRange, CenterPosition.X + RandRange);
	float RandomY = FMath::RandRange(CenterPosition.Y - RandRange, CenterPosition.Y + RandRange);
	return FVector(RandomX, RandomY, CenterPosition.Z);
}

bool AITForbiddenArea::IsInSafeArea(const AActor* Actor)
{
	if (HasAuthority())
	{
		if (IsValid(Actor))
		{
			FVector V0 = Actor->GetActorLocation();
			FVector V1 = this->GetActorLocation();
			float Distance2DSqure = ((V0.X - V1.X) * (V0.X - V1.X)) + ((V0.Y - V1.Y) * (V0.Y - V1.Y));
			float Radius = GetActorScale().X * 100.0f / 2;
			float RadiusSqure = Radius * Radius;
			return Distance2DSqure <= RadiusSqure;
		}
	}
	return false;
}

void AITForbiddenArea::ApplyDamage(UAbilitySystemComponent* ASC)
{
	if (HasAuthority())
	{
		if (IsValid(ASC))
		{
			const FITForbiddenRoundInfo& Info = GetCurrentRoundInfo();
			if (IsValid(Info.DamageEffect))
			{
				FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Info.DamageEffect, 1, ASC->MakeEffectContext());
				ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}
}
