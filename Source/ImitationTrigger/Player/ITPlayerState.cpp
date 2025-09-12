#include "Player/ITPlayerState.h"
#include "Player/ITPlayerController.h"
#include "Character/ITCharacter.h"
#include "Character/ITPawnData.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"

AITPlayerState::AITPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UITAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// AbilitySystem 네트워크 관련: needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);


	// PlayerState와 Pawn(Chracter)가 모두 준비되었을 때 호출되는 Delegate
	OnPawnSet.AddDynamic(this, &ThisClass::OnReadyPawnData);
}

AITPlayerController* AITPlayerState::GetITPlayerController() const
{
	return Cast<AITPlayerController>(GetOwner());
}

AITCharacter* AITPlayerState::GetITCharacter() const
{
	return Cast<AITCharacter>(GetPawn());
}

UAbilitySystemComponent* AITPlayerState::GetAbilitySystemComponent() const
{
	return GetITAbilitySystemComponent();
}

void AITPlayerState::OnReadyPawnData(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	AITCharacter* ITCharacter = GetITCharacter();
	if (IsValid(ITCharacter))
	{
		AActor* ComponentOwner = this;
		AActor* Avatar = ITCharacter;
		AbilitySystemComponent->InitAbilityActorInfo(ComponentOwner, Avatar);

		const UITPawnData* PawnData = ITCharacter->GetPawnData();
		if (IsValid(PawnData))
		{
			for (const UITAbilitySet* AbilitySet : PawnData->AbilitySets)
			{
				AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles);
			}
		}
	}
}
