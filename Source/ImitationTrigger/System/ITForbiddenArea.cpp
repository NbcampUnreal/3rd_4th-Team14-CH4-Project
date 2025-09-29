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
	bIsProgressing = false;
}

void AITForbiddenArea::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Round);
	DOREPLIFETIME(ThisClass, CenterPosition);
	DOREPLIFETIME(ThisClass, RadiusScale);
}

void AITForbiddenArea::Tick(float DeltaTime)
{
	if (bIsProgressing)
	{

	}
}

const FITForbiddenRoundInfo& AITForbiddenArea::GetCurrentRoundInfo() const
{
	return RoundInfos[Round];
}

void AITForbiddenArea::BeginPlay()
{
	Super::BeginPlay();
	
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
		AreaDamageTimer.Invalidate();
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
	IT_LOG_ROLE(LogITNet, Log, TEXT("Round End..."));

	const FITForbiddenRoundInfo& CurrentRoundInfo = GetCurrentRoundInfo();
	GetWorldTimerManager().SetTimer(AreaProgressTimer, this, &ThisClass::OnRoundStartTimer, CurrentRoundInfo.ProgressDuration, false);

	bIsProgressing = 1;
}

void AITForbiddenArea::OnRoundStartTimer()
{
	Round += 1;

	const FITForbiddenRoundInfo& CurrentRoundInfo = GetCurrentRoundInfo();
	GetWorldTimerManager().SetTimer(AreaRoundWaitTimer, this, &ThisClass::OnRoundEndTimer, CurrentRoundInfo.WaitDuration, false);

	bIsProgressing = 0;

	IT_LOG_ROLE(LogITNet, Log, TEXT("Round Start"));
}

bool AITForbiddenArea::IsInSafeArea(const AActor* Actor)
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
	return false;
}

void AITForbiddenArea::ApplyDamage(UAbilitySystemComponent* ASC)
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
