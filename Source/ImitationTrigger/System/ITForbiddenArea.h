#pragma once

#include "GameFramework/Actor.h"
#include "ITForbiddenArea.generated.h"

class UStaticMeshComponent;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FITForbiddenRoundInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AreaPercentage = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffect;
};

UCLASS()
class IMITATIONTRIGGER_API AITForbiddenArea : public AActor
{
	GENERATED_BODY()
	
public:
	AITForbiddenArea();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	const FITForbiddenRoundInfo& GetCurrentRoundInfo() const { return RoundInfos[Round]; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> AreaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area|Config")
	TArray<FITForbiddenRoundInfo> RoundInfos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area|Config")
	float TickInterval = 1.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Round)
	int32 Round;

	UPROPERTY(ReplicatedUsing = OnRep_CenterPosition);
	FVector CenterPosition = FVector::ZeroVector;

	UPROPERTY(ReplicatedUsing = OnRep_RadiusScale);
	float RadiusScale = 100.0f;

	float OriginRadiusScale;
	
	UFUNCTION()
	void OnRep_Round();

	UFUNCTION()
	void OnRep_CenterPosition();

	UFUNCTION()
	void OnRep_RadiusScale();

protected:
	FTimerHandle AreaLogicTimer;

	UFUNCTION()
	void OnLogicTimer();

	bool IsInSafeArea(const AActor* Actor);
};
