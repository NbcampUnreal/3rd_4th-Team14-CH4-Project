#include "Cosmetics/ITCharacterPartComponent.h"
#include "Net/UnrealNetwork.h"

UITCharacterPartComponent::UITCharacterPartComponent(const FObjectInitializer& ObjectInitialize)
	: Super(ObjectInitialize)
{

}

void UITCharacterPartComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CharacterPartList);
}

FITCharacterPartHandle UITCharacterPartComponent::AddCharacterPart(const FITCharacterPart& NewPart)
{
	return FITCharacterPartHandle();
}

void UITCharacterPartComponent::RemoveCharacterPart(FITCharacterPartHandle Handle)
{

}

USkeletalMeshComponent* UITCharacterPartComponent::GetParentMeshComponent() const
{
	return nullptr;
}

USceneComponent* UITCharacterPartComponent::GetSceneComponentToAttachTo() const
{
	return nullptr;
}

void UITCharacterPartComponent::BroadcastChanged()
{

}

FGameplayTagContainer UITCharacterPartComponent::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	return FGameplayTagContainer();
}