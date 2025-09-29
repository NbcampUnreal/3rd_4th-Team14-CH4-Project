#include "System/ITForbiddenArea.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ITCharacter.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"

AITForbiddenArea::AITForbiddenArea()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	AreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AreaMesh"));

	Round = 0;
}

void AITForbiddenArea::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Round);
	DOREPLIFETIME(ThisClass, CenterPosition);
	DOREPLIFETIME(ThisClass, RadiusScale);
}

void AITForbiddenArea::BeginPlay()
{
	Super::BeginPlay();
	
	OriginRadiusScale = RadiusScale;

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(AreaLogicTimer, this, &ThisClass::OnLogicTimer, 1.0f, true);
	}
}

void AITForbiddenArea::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (AreaLogicTimer.IsValid())
	{
		AreaLogicTimer.Invalidate();
	}
}


void AITForbiddenArea::OnRep_Round()
{
}

void AITForbiddenArea::OnRep_CenterPosition()
{
}

void AITForbiddenArea::OnRep_RadiusScale()
{
}

void AITForbiddenArea::OnLogicTimer()
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
				if (IsValid(ITASC))
				{
					const FITForbiddenRoundInfo& Info = GetCurrentRoundInfo();
					if (IsValid(Info.DamageEffect))
					{
						FGameplayEffectSpecHandle EffectSpecHandle = ITASC->MakeOutgoingSpec(Info.DamageEffect, 1, ITASC->MakeEffectContext());
						ITASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
					}
				}
			}
		}
	}
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
