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

	UFUNCTION(BlueprintCallable)
	const FVector GetNextCenterPosition() const { return NextCenterPosition; }

	UFUNCTION(BlueprintCallable)
	const float GetNextRadiusScale() const { return NextRadiusScale; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> AreaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area|Config")
	TArray<FITForbiddenRoundInfo> RoundInfos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area|Config")
	float TickInterval = 0.5f;

	UPROPERTY(ReplicatedUsing = OnRep_Round)
	int32 Round;

	UPROPERTY(Replicated)
	int8 bIsProgressing;

	FVector CenterPosition = FVector::ZeroVector;
	float RadiusScale = 100.0f;

	UPROPERTY(Replicated)
	FVector NextCenterPosition = FVector::ZeroVector;

	UPROPERTY(Replicated)
	float NextRadiusScale = 100.0f;

	float OriginRadiusScale;
	
	UFUNCTION()
	void OnRep_Round();


protected:
	FTimerHandle AreaDamageTimer;
	FTimerHandle AreaRoundWaitTimer;
	FTimerHandle AreaProgressTimer;
	FTimerHandle AreaSyncTimer;
	FTimerHandle AreaNotifyTimer;

	float SyncInterval = 1.0f;

	UFUNCTION()
	void OnDamageTimer();

	UFUNCTION()
	void OnRoundEndTimer();

	UFUNCTION()
	void OnRoundStartTimer();

	UFUNCTION()
	void OnSyncTimer();

	UFUNCTION()
	void OnNotifyTimer();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SyncArea(FITSyncInfo SyncInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_NotifyAreaInfo(int32 InRound, float InRemainTime, FVector InCenterPosition, float InRadius, int8 bIsWait);

	FITSyncInfo ClientSyncInfo;

	void UpdateArea(const FVector& InCenterPosition, float InRadiusScale);
	FVector GenerateRandomCenterPosition();
	bool IsInSafeArea(const AActor* Actor);
	void ApplyDamage(UAbilitySystemComponent* ASC);
};
