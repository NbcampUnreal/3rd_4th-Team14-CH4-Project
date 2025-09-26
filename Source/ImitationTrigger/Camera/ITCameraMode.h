#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "ITCameraMode.generated.h"

namespace ITCameraModeTags
{
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CameraMode);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CameraMode_ADS);
}

class UITCameraMode;
class UITCameraComponent;

USTRUCT(BlueprintType)
struct FCameraModeWithTag
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UITCameraMode> CameraModeClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "CameraMode"))
	FGameplayTag CameraModeTag;
};

struct FITCameraModeView
{
	FITCameraModeView();

	void Blend(const FITCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

UENUM(BlueprintType)
enum class EITCameraModeBlendFunction : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};


UCLASS(Abstract, NotBlueprintable)
class IMITATIONTRIGGER_API UITCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UITCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	UITCameraComponent* GetITCameraComponent() const;
	AActor* GetTargetActor() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;

	FITCameraModeView View;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Calmpmax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	float BlendAlpha;

	float BlendWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;


	EITCameraModeBlendFunction BlendFunction;
};


UCLASS()
class UITCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	UITCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UITCameraMode* GetCameraModeInstance(TSubclassOf<UITCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<UITCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FITCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FITCameraModeView& OutCameraModeView) const;

	UPROPERTY()
	TArray<TObjectPtr<UITCameraMode>> CameraModeInstances;


	UPROPERTY()
	TArray<TObjectPtr<UITCameraMode>> CameraModeStack;
};
