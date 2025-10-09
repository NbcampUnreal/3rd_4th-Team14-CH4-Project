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


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};




