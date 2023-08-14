// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemObject.h"
#include "EquipItemObject.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "CharacterStateComponent.h"
#include "PlayerActorComponent.h"
#include "EnemyStateActorComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InventorySize = 20; //Default 20
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetOwner());
	if (Player)
	{
		CharacterComponent = Cast<UCharacterStateComponent>(Player->PlayerComponent);
	}

	if (Enemy)
	{
		CharacterComponent = Cast<UCharacterStateComponent>(Enemy->EnemyStateComponent);
	}
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	for (auto& item : DefaultInventory)
	{
		AddItem(item);
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::AddItem(UItemObject* item)
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

bool UInventoryComponent::RemoveItem(UItemObject* item)
{
	if (ItemInventory.Num() > 0)
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

bool UInventoryComponent::EquipItem(UEquipItemObject* Item)
{
	if (!Item->Equip)
	{
		Item->Equip = true;
		if (CharacterComponent->GetEquip(Item) != nullptr)
		{
			if (Item->ItemEnum == CharacterComponent->GetEquip(Item)->ItemEnum)
			{
				UnEquipItem(CharacterComponent->GetEquip(Item));
			}
		}
		CharacterComponent->SetEquip(Item);
		RemoveItem(Item);
		EquipInventory.Add(Item);
		return true;
	}
	else
		return false;
}
bool UInventoryComponent::UnEquipItem(UEquipItemObject* Item)
{
	if (Item->Equip)
	{
		Item->Equip = false;
		AddItem(Item);
		EquipInventory.RemoveSingle(Item);
		return true;
	}
	else
		return false;
}