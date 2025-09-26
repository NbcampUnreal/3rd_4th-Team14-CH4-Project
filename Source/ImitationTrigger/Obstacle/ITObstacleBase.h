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

	// 시작 위치
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Location|Components")
	FVector StartLocation;

	// 회전
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float RotationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float RotationSpeedX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float RotationSpeedY;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float RotationSpeedZ;

	// 왕복 운동 함수 추가
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float Amplitude;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float Speed;

	// 원형 회전 운동
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float Radius;
	// float Speed : 위에 있는 멤버 Speed를 사용할 예정

	// 지그재그 운동
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Components")
	float Frequency;
	// float Speed;


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};




