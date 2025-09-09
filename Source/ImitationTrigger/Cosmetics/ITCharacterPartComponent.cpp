#include "Cosmetics/ITCharacterPartComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UITCharacterPartComponent::UITCharacterPartComponent(const FObjectInitializer& ObjectInitialize)
	: Super(ObjectInitialize), CharacterPartList(this)
{
	SetIsReplicatedByDefault(true);
}

void UITCharacterPartComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CharacterPartList);
}

FITCharacterPartHandle UITCharacterPartComponent::AddCharacterPart(const FITCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void UITCharacterPartComponent::RemoveCharacterPart(FITCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}

USkeletalMeshComponent* UITCharacterPartComponent::GetParentMeshComponent() const
{
	ACharacter* OwnerCharacter = GetOwner<ACharacter>();
	if (IsValid(OwnerCharacter))
	{
		return OwnerCharacter->GetMesh();
	}
	return nullptr;
}

USceneComponent* UITCharacterPartComponent::GetSceneComponentToAttachTo() const
{
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetParentMeshComponent())
	{
		return SkeletalMeshComponent;
	}
	else if (AActor* Owner = GetOwner())
	{
		return Owner->GetRootComponent();
	}
	else
	{
		return nullptr;
	}
}

// CharacterPart가 변했을 때, 그 CharacterPart에 맞는 Skeleton을 찾아 적용한다.
void UITCharacterPartComponent::BroadcastChanged()
{
	const bool bReinitPose = false;

	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);
		MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPose);

		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPose);
		}
	}
}

FGameplayTagContainer UITCharacterPartComponent::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	return FGameplayTagContainer();
}