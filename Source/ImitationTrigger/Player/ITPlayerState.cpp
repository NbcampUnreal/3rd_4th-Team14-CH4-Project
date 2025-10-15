#include "Player/ITPlayerState.h"
#include "Player/ITPlayerController.h"
#include "Character/ITCharacter.h"
#include "Character/ITPawnData.h"
#include "Character/ITPawnDataList.h"
#include "Item/Weapon/ITWeaponManagerComponent.h"
#include "AbilitySystem/Attributes/ITHealthSet.h"
#include "AbilitySystem/Attributes/ITAmmoSet.h"
#include "AbilitySystem/Attributes/ITCombatSet.h"
#include "AbilitySystem/Attributes/ITAttributeTableRow.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Engine/ActorChannel.h"
#include "Item/ITItemInstance.h"
#include "GameModes/ITBattleGameMode.h"
#include "UObject/ObjectPtr.h"
#include "Item/ITItemManagerComponent.h"
#include "Net/UnrealNetwork.h"


AITPlayerState::AITPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UITAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	WeaponManagerComponent = CreateDefaultSubobject<UITWeaponManagerComponent>(TEXT("WeaponManagerComponent"));
	WeaponManagerComponent->SetIsReplicated(true);

	ItemManagerComponent = CreateDefaultSubobject<UITItemManagerComponent>(TEXT("ItemManagerComponent"));
	ItemManagerComponent->SetIsReplicated(true);

	// AbilitySystem 네트워크 관련: needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);

	HealthSet = CreateDefaultSubobject<UITHealthSet>(TEXT("HealthSet"));
	AmmoSet = CreateDefaultSubobject<UITAmmoSet>(TEXT("AmmoSet"));
	CombatSet = CreateDefaultSubobject<UITCombatSet>(TEXT("CombatSet"));

	bIsAlive = true;

	// PlayerState와 Pawn(Chracter)가 모두 준비되었을 때 호출되는 Delegate
	OnPawnSet.AddDynamic(this, &ThisClass::OnReadyPawnData);

	Rank = -1;
	StartTimeSeconds = 0.0f;
	EndTimeSeconds = 0.0f;

	CachedPawnData = nullptr;
}

void AITPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AITPlayerState::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(WeaponManagerComponent))
	{
		AITCharacter* ITCharacter = GetITCharacter();
		if (IsValid(ITCharacter))
		{
			UITCharacterAnimComponent* AnimComponent = ITCharacter->GetITCharacterAnimComponent();
			if (IsValid(AnimComponent))
			{
				WeaponManagerComponent->OnCurrentWeaponTypeChanged.RemoveDynamic(AnimComponent, &UITCharacterAnimComponent::OnUpdateCurrentWeapon);
			}
		}
	}
}

void AITPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsAlive);
	DOREPLIFETIME(ThisClass, Rank);
	DOREPLIFETIME(ThisClass, StartTimeSeconds);
	DOREPLIFETIME(ThisClass, EndTimeSeconds);
	DOREPLIFETIME(ThisClass, CachedPawnData);
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

const UITPawnData* AITPlayerState::GetPawnData() const
{
	if (IsValid(CachedPawnData))
	{
		return CachedPawnData;
	}
	else
	{
		if (IsValid(GetITCharacter()))
		{
			return GetITCharacter()->GetPawnData();
		}
		return nullptr;
	}
}

void AITPlayerState::SetStartTimeSeconds(float InStartTimeSeconds)
{
	StartTimeSeconds = InStartTimeSeconds;
}

void AITPlayerState::SetEndTimeSeconds(float InEndTimeSeconds)
{
	EndTimeSeconds = InEndTimeSeconds;
}

float AITPlayerState::GetSurviveTimeSeconds() const
{
	return FMath::Max(0.0f, (EndTimeSeconds - StartTimeSeconds));
}

bool AITPlayerState::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (WeaponManagerComponent)
	{
		UITItemInstance* MainWeapon = WeaponManagerComponent->GetMainWeaponInstance();
		if (IsValid(MainWeapon))
		{
			bWroteSomething |= Channel->ReplicateSubobject(MainWeapon, *Bunch, *RepFlags);
		}

		UITItemInstance* SubWeapon = WeaponManagerComponent->GetSubWeaponInstance();
		if (IsValid(SubWeapon))
		{
			bWroteSomething |= Channel->ReplicateSubobject(SubWeapon, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void AITPlayerState::OnReadyPawnData(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	AITCharacter* ITCharacter = GetITCharacter();
	if (IsValid(ITCharacter) && IsValid(ITCharacter->GetPawnData()))
	{
		CachedPawnData = ITCharacter->GetPawnData();

		InitAbilitySystemComponent();

		ITCharacter->SetBodyMeshes();
		ITCharacter->SetAnimLayerRules();

		BindWeaponChanged();
	}
}

void AITPlayerState::InitAttributeSet(UDataTable* InitDataTable)
{
	if (IsValid(InitDataTable))
	{
		FString ContextString(TEXT("OnReadyPawnDataContext"));
		TArray<FITAttributeTableRow*> InitRows;
		InitDataTable->GetAllRows(ContextString, InitRows);

		for (const FITAttributeTableRow* Row : InitRows)
		{
			if (Row && Row->Attribute.IsValid())
			{
				AbilitySystemComponent->SetNumericAttributeBase(Row->Attribute, Row->InitValue);

				if (Row->CurrentAttribute.IsValid())
				{
					AbilitySystemComponent->SetNumericAttributeBase(Row->CurrentAttribute, Row->InitValue);
				}
			}
		}
	}
}

void AITPlayerState::BindAttributeDelegate()
{
	UITAbilitySystemComponent* ITASC = GetITAbilitySystemComponent();
	if (IsValid(ITASC))
	{
		FGameplayAttribute Attribute = UITHealthSet::GetHealthAttribute();
		ITASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &AITPlayerState::OnHealthChanged);
	}
}

void AITPlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (HasAuthority())
	{
		AITCharacter* ITCharacter = GetITCharacter();
		if (IsValid(ITCharacter))
		{
			const float Health = Data.NewValue;
			if (Health <= 0)
			{
				if (IsValid(ITCharacter))
				{
					bIsAlive = false;

					if (AITBattleGameMode* GameMode = Cast<AITBattleGameMode>(GetWorld()->GetAuthGameMode()))
					{
						GameMode->OnPlayerDeath(this);
					}
					ITCharacter->MulticastRPC_OnDead();
				}
			}
		}
	}
}

void AITPlayerState::InitAbilitySystemComponent()
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

			InitAttributeSet(PawnData->InitDataTable);
		}

		if (HasAuthority())
		{
			BindAttributeDelegate();
		}
	}
}

void AITPlayerState::BindWeaponChanged()
{
	AITCharacter* ITCharacter = GetITCharacter();
	if (IsValid(ITCharacter))
	{
		if (IsValid(WeaponManagerComponent))
		{
			UITCharacterAnimComponent* AnimComponent = ITCharacter->GetITCharacterAnimComponent();
			if (IsValid(AnimComponent))
			{
				WeaponManagerComponent->OnCurrentWeaponTypeChanged.AddUniqueDynamic(AnimComponent, &UITCharacterAnimComponent::OnUpdateCurrentWeapon);
			}
		}
	}
}
