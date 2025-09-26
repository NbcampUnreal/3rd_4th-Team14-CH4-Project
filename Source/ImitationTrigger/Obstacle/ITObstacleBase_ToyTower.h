
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
	// 0. 왕복운동
	//FVector StartLocation;

	////UPROPERTY(EditAnywhere, Category = "Elevator|Movement")
	////float MoveDistance;

	//// 부모클래스에 Speed가 이미 정의되어 있음.
	////UPROPERTY(EditAnywhere, Category = "Elevator|Movement")
	////float Speed = 100.0f; 

	//UPROPERTY(EditAnywhere, Category = "Elevator|Movement")
	//float PauseDuration; // 멈췄다가 다시 시작할 시간

	//bool bMovingUp;
	//bool bIsActive;

	//FTimerHandle ResumeTimerHandle;


protected:
	// 1.(1)왕복 안함.
	// 베이스(AITObstacleBase)에 StartLocation(FVector)이 이미 있으니 여기서는 타겟 위치만 따로 가진다.
	//FVector TargetLocation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	//float MoveDistance = 200.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	//float Speed = 3.0f;   // 보간 속도

	//bool bIsMoving = true;

	// 1.(2)
protected:
	FVector StartLocation;
	FVector TargetLocation;

	// 시간 기반 Lerp
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveDistance = 150.f; // 위로 얼마나 올라갈지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RiseDuration = 0.5f;  // 몇 초 동안 올라갈지

	bool bIsMoving = true;


	void ResumeMovement(); // 다시 움직이는 함수

};