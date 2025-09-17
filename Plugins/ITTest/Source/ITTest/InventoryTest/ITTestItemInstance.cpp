// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestItemInstance.h"
#include "ITTestItemData.h"
#include "Net/UnrealNetwork.h"

bool UITTestItemInstance::IsSupportedForNetworking() const
{
	return true;
}

void UITTestItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UITTestItemInstance, ItemData);
	DOREPLIFETIME(UITTestItemInstance, Quantity);
}

void UITTestItemInstance::InitItemInstance(UITTestItemData* InitItemData, int32 InitQuantity)
{
	ItemData = InitItemData;
	Quantity = InitQuantity;
}

void UITTestItemInstance::OnRep_ItemData()
{
	OnItemInstanceUpdated.Broadcast();
}

void UITTestItemInstance::OnRep_Quantity()
{
	OnItemInstanceUpdated.Broadcast();
}
