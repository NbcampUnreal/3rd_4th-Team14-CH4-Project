// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ITItemFragmentBPFL.h"

bool UITItemFragmentBPFL::GetFragment_Grenade(const UITItemDefinition* ItemDef, FITItemFragment_Grenade& Value)
{
	if (!ItemDef) return false;
	if (const FITItemFragment_Grenade* Fragment_Grenade = ItemDef->GetFragment<FITItemFragment_Grenade>())
	{
		Value = *Fragment_Grenade;
		return true;
	}
	return false;
}
