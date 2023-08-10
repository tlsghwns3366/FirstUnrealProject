// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryComponent.h"
#include "EquipItemObject.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InventorySize = 20; //Default 20
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UPlayerInventoryComponent::EquipItem(UEquipItemObject* Item)
{
	if (!Item->Equip)
	{
		Item->World = GetWorld();
		Item->Inventory = this;
		Item->Equip = true;
		EquipInventory.Add(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}
bool UPlayerInventoryComponent::UnEquipItem(UEquipItemObject* Item)
{
	if (Item->Equip)
	{
		Item->World = nullptr;
		Item->Inventory = nullptr;
		Item->Equip = false;
		ItemInventory.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}