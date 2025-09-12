#pragma once

#include "GameFramework/Character.h"
#include "ITCharacter.generated.h"

class UITPawnData;

UCLASS()
class IMITATIONTRIGGER_API AITCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AITCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_PawnData, Category = "PawnData")
	TObjectPtr<const UITPawnData> PawnData;

private:
	UFUNCTION()
	void OnRep_PawnData();
};
