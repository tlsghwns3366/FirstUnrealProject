// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemObject.h"
#include "EquipItemObject.h"
#include "CustomCharacter.h"
#include "CharacterStateComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InventorySize = 20; //Default 20
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
	ACustomCharacter* Character = Cast<ACustomCharacter>(GetOwner());
	if (IsValid(Character))
	{
		MainStateComponent = Character->MainStateComponent;
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::AddItem(UItemObject* Item)
{
	if (ItemInventory.Num() < InventorySize)
	{
		Item->World = GetWorld();
		Item->Inventory = this;
		ItemInventory.Add(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}

bool UInventoryComponent::RemoveItem(UItemObject* Item)
{
	if (ItemInventory.Num() > 0)
	{
		Item->World = nullptr;
		Item->Inventory = nullptr;
		ItemInventory.RemoveSingle(Item);
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
		if (MainStateComponent != nullptr && MainStateComponent->GetEquip(Item) != nullptr)
		{
			if (Item->ItemEnum == MainStateComponent->GetEquip(Item)->ItemEnum)
			{
				UnEquipItem(MainStateComponent->GetEquip(Item));
			}
		}
		Item->Equip = true;
		MainStateComponent->SetEquip(Item, Item->ItemEnum);
		ItemInventory.RemoveSingle(Item);
		EquipInventory.Add(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}
bool UInventoryComponent::UnEquipItem(UEquipItemObject* Item)
{
	if (Item->Equip)
	{
		MainStateComponent->SetEquip(nullptr, Item->ItemEnum);
		Item->Equip = false;
		EquipInventory.RemoveSingle(Item);
		ItemInventory.Add(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}