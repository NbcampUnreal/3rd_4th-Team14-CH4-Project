#pragma once

#include "CoreMinimal.h"
#include "ITCameraMode.h"
#include "Camera/CameraComponent.h"
#include "ITCameraComponent.generated.h"


class UITCameraModeStack;

template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UITCameraMode>, FITCameraModeDelegate);

UCLASS()
class IMITATIONTRIGGER_API UITCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UITCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UITCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UITCameraComponent>() : nullptr); }


	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();

	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	UPROPERTY()
	TObjectPtr<UITCameraModeStack> CameraModeStack;


	FITCameraModeDelegate DetermineCameraModeDelegate;

};



// ITHeroComponent에 정의할 내용

//if (bIsLocallyControlled && PawnData)
//{
//	if (UITCameraComponent* CameraComponent = UITCameraComponent::FindCameraComponent(Pawn))
//	{
//		CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
//	}
//}

//TSubclassOf<UITCameraMode> UITHeroComponent::DetermineCameraMode() const
//{
//	const APawn* Pawn = GetPawn<APawn>();
//	if (!Pawn)
//	{
//		return nullptr;
//	}
//
//	if (UITPawnExtensionComponent* PawnExtComp = UITPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
//	{
//		if (const UHakPawnData* PawnData = PawnExtComp->GetPawnData<UITPawnData>())
//		{
//			return PawnData->DefaultCameraMode;
//		}
//	}
//
//	return nullptr;
//}
