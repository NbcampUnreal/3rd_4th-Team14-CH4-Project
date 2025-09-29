#pragma once

#include "GameFramework/Actor.h"
#include "ITForbiddenArea.generated.h"

class UStaticMeshComponent;
class UGameplayEffect;
class UAbilitySystemComponent;

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
	virtual void Tick(float DeltaTime) override;

	const FITForbiddenRoundInfo& GetCurrentRoundInfo() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> AreaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area|Config")
	TArray<FITForbiddenRoundInfo> RoundInfos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area|Config")
	float TickInterval = 1.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Round)
	int32 Round;

	UPROPERTY(Replicated)
	int8 bIsProgressing;

	UPROPERTY(Replicated);
	FVector CenterPosition = FVector::ZeroVector;

	UPROPERTY(Replicated);
	float RadiusScale = 100.0f;

	UPROPERTY(Replicated);
	FVector NextCenterPosition = FVector::ZeroVector;

	UPROPERTY(Replicated);
	float NextRadiusScale = 100.0f;

	float OriginRadiusScale;
	
	UFUNCTION()
	void OnRep_Round();


protected:
	FTimerHandle AreaDamageTimer;
	FTimerHandle AreaRoundWaitTimer;
	FTimerHandle AreaProgressTimer;

	UFUNCTION()
	void OnDamageTimer();

	UFUNCTION()
	void OnRoundTimer();

	bool IsInSafeArea(const AActor* Actor);
	void ApplyDamage(UAbilitySystemComponent* ASC);
};
