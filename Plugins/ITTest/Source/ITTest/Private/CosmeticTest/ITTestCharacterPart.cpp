#include "CosmeticTest/ITTestCharacterPart.h"
#include "Character/ITCharacter.h"

void UITTestCharacterPart::Blueprint_ClearCharacterParts()
{
	Super::ClearCharacterParts();
}

void UITTestCharacterPart::Blueprint_AddCharacterPart_Internal(const FITCharacterPart& NewPart)
{
	AddCharacterPart_Internal(NewPart);
}

void UITTestCharacterPart::AddCharacterPart_Internal(const FITCharacterPart& NewPart)
{
	AITCharacter* ITCharacter = GetOwner<AITCharacter>();
	if (IsValid(ITCharacter))
	{
		TArray<FITCharacterPartHandle>& OutHandles = ITCharacter->GetCharacterPartHandles();
		OutHandles.Add(AddCharacterPart(NewPart));
	}
}

