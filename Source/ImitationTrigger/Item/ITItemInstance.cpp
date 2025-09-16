// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemInstance.h"
#include "Net/UnrealNetwork.h"

bool UITItemInstance::IsSupportedForNetworking() const
{
	return true;
}

void UITItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UITItemInstance, ItemDefinition);
	DOREPLIFETIME(UITItemInstance, ItemQuantity);
}

void UITItemInstance::InitItemInstance(UITItemDefinition* InItemDefinition, int32 InitQuantity)
{
	ItemDefinition = InItemDefinition;
	ItemQuantity = InitQuantity;
}

UITItemDefinition* UITItemInstance::GetItemDefinition() const
{
	return ItemDefinition;
}
