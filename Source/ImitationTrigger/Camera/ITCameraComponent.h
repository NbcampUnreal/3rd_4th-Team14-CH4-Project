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
