// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyInventoryComponent.h"
#include "ItemObject.h"

UEnemyInventoryComponent::UEnemyInventoryComponent()
{
	InventorySize = 10;
}


bool UEnemyInventoryComponent::AddItem(UItemObject* item)
{
	float RandomDrop = FMath::RandRange(0.f,100000.f);
	if (item->DropChance  > RandomDrop/100000.f)
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