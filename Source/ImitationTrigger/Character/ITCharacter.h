#pragma once

#include "GameFramework/Character.h"
#include "ITCharacter.generated.h"

class UITCameraComponent;

UCLASS()
class IMITATIONTRIGGER_API AITCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AITCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Character")
	TObjectPtr<UITCameraComponent> CameraComponent;

};
