//1.
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "ITObstacleBase.generated.h"
//
//UCLASS()
//class IMITATIONTRIGGER_API AITObstacleBase : public AActor
//{
//	GENERATED_BODY()
//
//public:	
//	AITObstacleBase();
//
//protected:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
//	USceneComponent* SceneRoot;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
//	UStaticMeshComponent* StaticMeshComp;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
//	float Speed;
//
//
//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void Tick(float DeltaTime) override;
//
//
//};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ITObstacleBase.generated.h"

UCLASS()
class IMITATIONTRIGGER_API AITObstacleBase : public AActor
{
	GENERATED_BODY()

public:
	AITObstacleBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	// ���� ��ġ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Location|Components")
	FVector StartLocation;

	// ȸ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float RotationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float RotationSpeedX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float RotationSpeedY;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float RotationSpeedZ;

	// �պ� � �Լ� �߰�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float Amplitude;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float Speed;

	// ���� ȸ�� �
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float Radius;
	// float Speed : ���� �ִ� ��� Speed�� ����� ����

	// ������� �
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float Frequency;
	// float Speed;


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};




