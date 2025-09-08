#pragma once

#include "GameFramework/Actor.h"
#include "ITTestActor.generated.h"

UCLASS()
class ITTEST_API AITTestActor : public AActor
{
	GENERATED_BODY()
	
public:
	AITTestActor();
	virtual void BeginPlay() override;
};
