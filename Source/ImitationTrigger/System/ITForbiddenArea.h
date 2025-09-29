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
	float WaitDuration = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProgressDuration = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AreaPercentage = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffect;
};

USTRUCT()
struct FITSyncInfo
{
	GENERATED_BODY()

	UPROPERTY();
	FVector CenterPosition;

	UPROPERTY();
	float RadiusScale;

	UPROPERTY();
	FVector NextCenterPosition;

	UPROPERTY();
	float NextRadiusScale;

	UPROPERTY();
	float TotalTime;

	UPROPERTY();
	float RemainTime;
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
	const FITForbiddenRoundInfo& GetNextRoundInfo() const;

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

	FVector CenterPosition = FVector::ZeroVector;
	FVector NextCenterPosition = FVector::ZeroVector;

	float RadiusScale = 100.0f;
	float NextRadiusScale = 100.0f;
	float OriginRadiusScale;
	
	UFUNCTION()
	void OnRep_Round();


protected:
	FTimerHandle AreaDamageTimer;
	FTimerHandle AreaRoundWaitTimer;
	FTimerHandle AreaProgressTimer;
	FTimerHandle AreaSyncTimer;

	float SyncInterval = 1.0f;

	UFUNCTION()
	void OnDamageTimer();

	UFUNCTION()
	void OnRoundEndTimer();

	UFUNCTION()
	void OnRoundStartTimer();

	UFUNCTION()
	void OnSyncTimer();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SyncArea(FITSyncInfo SyncInfo);

	FITSyncInfo ClientSyncInfo;

	void UpdateArea(const FVector& InCenterPosition, float InRadiusScale);
	FVector GenerateRandomCenterPosition();
	bool IsInSafeArea(const AActor* Actor);
	void ApplyDamage(UAbilitySystemComponent* ASC);
};
