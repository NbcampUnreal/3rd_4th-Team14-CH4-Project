#pragma once

#include "CoreMinimal.h"
#include "Cosmetics/ITCharacterPartComponent.h"
#include "ITTestCharacterPart.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ITTEST_API UITTestCharacterPart : public UITCharacterPartComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Blueprint_ClearCharacterParts();

	UFUNCTION(BlueprintCallable)
	void Blueprint_AddCharacterPart_Internal(const FITCharacterPart& NewPart);

	void AddCharacterPart_Internal(const FITCharacterPart& NewPart);
};
