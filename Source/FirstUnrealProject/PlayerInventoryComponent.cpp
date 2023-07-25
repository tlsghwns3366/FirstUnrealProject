// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryComponent.h"
#include "ItemObject.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InventorySize = 20; //Default 20
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	for (auto& item : DefaultInventory)
	{
		AddItem(item);
	}
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UPlayerInventoryComponent::AddItem(UItemObject* item)
{
	if (ItemInventory.Num() < InventorySize)
	{
		item->World = GetWorld();
		item->Inventory = this;
		ItemInventory.Add(item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}

bool UPlayerInventoryComponent::RemoveItem(UItemObject* item)
{
	if (ItemInventory.Num() < InventorySize)
	{
		item->World = nullptr;
		item->Inventory = nullptr;
		ItemInventory.RemoveSingle(item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}
