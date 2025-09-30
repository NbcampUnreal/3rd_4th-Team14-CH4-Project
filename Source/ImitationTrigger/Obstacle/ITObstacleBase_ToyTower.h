
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Obstacle/ITObstacleBase.h"
//#include "ITObstacleBase_ToyTower.generated.h"
//
//
//
//UCLASS()
//class IMITATIONTRIGGER_API AITObstacleBase_ToyTower : public AITObstacleBase
//{
//	GENERATED_BODY()
//	
//
//public:
//	AITObstacleBase_ToyTower();
//
//protected:
//	virtual void BeginPlay() override;
//	virtual void Tick(float DeltaTime) override;
//
//private:
//	FVector StartLocation;
//	FVector TargetLocation;
//
//	UPROPERTY(EditAnywhere, Category = "Movement")
//	float MoveDistance = 200.0f;
//
//	bool bIsMoving;
//
//};
//


#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITObstacleBase.h"
#include "ITObstacleBase_ToyTower.generated.h"

UCLASS()
class IMITATIONTRIGGER_API AITObstacleBase_ToyTower : public AITObstacleBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AITObstacleBase_ToyTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 0. �պ��
	//FVector StartLocation;

	////UPROPERTY(EditAnywhere, Category = "Elevator|Movement")
	////float MoveDistance;

	//// �θ�Ŭ������ Speed�� �̹� ���ǵǾ� ����.
	////UPROPERTY(EditAnywhere, Category = "Elevator|Movement")
	////float Speed = 100.0f; 

	//UPROPERTY(EditAnywhere, Category = "Elevator|Movement")
	//float PauseDuration; // ����ٰ� �ٽ� ������ �ð�

	//bool bMovingUp;
	//bool bIsActive;

	//FTimerHandle ResumeTimerHandle;


protected:
	// 1.(1)�պ� ����.
	// ���̽�(AITObstacleBase)�� StartLocation(FVector)�� �̹� ������ ���⼭�� Ÿ�� ��ġ�� ���� ������.
	//FVector TargetLocation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	//float MoveDistance = 200.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	//float Speed = 3.0f;   // ���� �ӵ�

	//bool bIsMoving = true;

	// 1.(2)
protected:
	FVector StartLocation;
	FVector TargetLocation;

	// �ð� ��� Lerp
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveDistance = 150.f; // ���� �󸶳� �ö���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RiseDuration = 0.5f;  // �� �� ���� �ö���

	bool bIsMoving = true;


	void ResumeMovement(); // �ٽ� �����̴� �Լ�

};