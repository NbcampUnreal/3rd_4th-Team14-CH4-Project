#include "Player/ITPlayerState.h"
#include "Player/ITPlayerController.h"
#include "Character/ITCharacter.h"
#include "Character/ITPawnData.h"
#include "Item/Weapon/ITWeaponManagerComponent.h"
#include "AbilitySystem/Attributes/ITHealthSet.h"
#include "AbilitySystem/Attributes/ITAttributeTableRow.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"


AITPlayerState::AITPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UITAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	WeaponManagerComponent = CreateDefaultSubobject<UITWeaponManagerComponent>(TEXT("WeaponManagerComponent"));
	WeaponManagerComponent->SetIsReplicated(true);

	// AbilitySystem 네트워크 관련: needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);

	HealthSet = CreateDefaultSubobject<UITHealthSet>(TEXT("HealthSet"));

	// PlayerState와 Pawn(Chracter)가 모두 준비되었을 때 호출되는 Delegate
	OnPawnSet.AddDynamic(this, &ThisClass::OnReadyPawnData);
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

			InitAttributeSet(PawnData->InitDataTable);
		}

		if (HasAuthority())
		{
			BindAttributeDelegate();
		}

		ITCharacter->SetBodyMeshes();
		ITCharacter->SetAnimLayerRules();

		if (IsValid(WeaponManagerComponent))
		{
			UITCharacterAnimComponent* AnimComponent = ITCharacter->GetITCharacterAnimComponent();
			if (IsValid(AnimComponent))
			{
				WeaponManagerComponent->OnCurrentWeaponTypeChanged.AddDynamic(AnimComponent, &UITCharacterAnimComponent::OnUpdateCurrentWeapon);
			}
		}
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
	const float Health = Data.NewValue;
	if (Health <= 0)
	{
		// TEMPORAL: 임시 사망 코드
		// TODO: 기절(down) 등 구현
		if (IsValid(GetPawn()))
		{
			GetPawn()->Destroy();
		}
	}
}
