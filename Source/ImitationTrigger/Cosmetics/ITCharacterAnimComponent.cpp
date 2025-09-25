#include "Cosmetics/ITCharacterAnimComponent.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Character/ITCharacter.h"

namespace ITCameraModeTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AnimLayer, "AnimLayer", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AnimLayer_UnArm, "AnimLayer.UnArm", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AnimLayer_Pistol, "AnimLayer.Pistol", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AnimLayer_Rifle, "AnimLayer.Rifle", "");
}

UITCharacterAnimComponent::UITCharacterAnimComponent(const FObjectInitializer& ObjectInitialize)
	: Super(ObjectInitialize)
{
}

void UITCharacterAnimComponent::SetAnimLayerRules(TArray<FITAnimLayerEntry> Rules)
{
	AnimLayerRules = Rules;
	if (AnimLayerRules.Num() > 0)
	{
		DefaultAnimLayer = AnimLayerRules[0].AnimLayerClass;
	}
	UpdateAnimLayer();
}

TSubclassOf<UAnimInstance> UITCharacterAnimComponent::FindBestMatchAnimLayer()
{
	AITCharacter* ITCharacter = GetOwner<AITCharacter>();
	if (IsValid(ITCharacter))
	{
		UAbilitySystemComponent* ASC = ITCharacter->GetAbilitySystemComponent();
		if (IsValid(ASC))
		{
			FGameplayTagContainer Tags = ASC->GetOwnedGameplayTags();
			for (const FITAnimLayerEntry& Element : AnimLayerRules)
			{
				if (Element.AnimLayerTag.MatchesAnyExact(Tags))
				{
					return Element.AnimLayerClass;
				}
			}
			return DefaultAnimLayer;
		}
	}
	return nullptr;
}

void UITCharacterAnimComponent::UpdateAnimLayer()
{
	TSubclassOf<UAnimInstance> AnimLayerClass = FindBestMatchAnimLayer();
	if (IsValid(AnimLayerClass))
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetParentMeshComponent();
		if (IsValid(SkeletalMeshComponent))
		{
			UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
			AnimInstance->LinkAnimClassLayers(AnimLayerClass);
		}
	}
}

void UITCharacterAnimComponent::OnUpdateCurrentWeapon(ECurrentWeaponSlot CurrentWeaponType)
{
	UpdateAnimLayer();
}

USkeletalMeshComponent* UITCharacterAnimComponent::GetParentMeshComponent() const
{
	ACharacter* OwnerCharacter = GetOwner<ACharacter>();
	if (IsValid(OwnerCharacter))
	{
		return OwnerCharacter->GetMesh();
	}
	return nullptr;
}
