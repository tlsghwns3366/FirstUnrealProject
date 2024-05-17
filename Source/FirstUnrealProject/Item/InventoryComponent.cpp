// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemObject.h"
#include "EquipItemObject.h"
#include "ConsumableItemObject.h"

#include "Character/CustomCharacter.h"
#include "Character/PlayerCharacter.h"
#include "ActorComponent/CharacterStateComponent.h"
#include "ActorComponent/PlayerMessageComponent.h"
#include "ActorComponent/QuickSlotComponent.h"

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
	initializeInventory();
	for (int32 Index = 0;Index< DefaultInventory.Num();Index++)
	{
		AddItem(DefaultInventory[Index]);		
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UInventoryComponent::AddItem(UItemObject* Item)
{
	int32 ItemIndex = GetInventorySlotIndex();
	if (ItemIndex != -1)
	{
		if (Item->IsStack)
		{
			for (UItemObject* InventoryItem : ItemInventory)
			{
				if (Item->ItemName == InventoryItem->ItemName)
				{
					InventoryItem->Count = InventoryItem->Count + Item->Count;
					Item->ConditionalBeginDestroy();
					OnInventoryUpdated.Broadcast();
					return true;
				}
			}
		}
		Item->World = GetWorld();
		Item->Inventory = this;
		Item->InventorySlotNumber = ItemIndex;
		ItemInventory[ItemIndex] = Item;
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventoryComponent::RemoveItem(UItemObject* Item)
{
	if (Item != nullptr)
	{
		Item->World = nullptr;
		Item->Inventory = nullptr;
		if (Item->QuickSlotNumber != -1.f)
			Cast<APlayerCharacter>(GetOwner())->QuickSlotComponent->RemoveObject(Item->QuickSlotNumber);
		SetBlankInventory(Item->InventorySlotNumber);
		Item->InventorySlotNumber = -1;

		OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}

bool UInventoryComponent::EquipItemRemove(UEquipItemObject* Item)
{
	if (Item != nullptr)
	{
		Item->UnEquipItem(Item);
		RemoveItem(Item);
		return true;
	}
	else
		return false;
}

UItemObject* UInventoryComponent::GetIndexItem(int32 Index)
{
	if (Index == -1)
		return nullptr;
	else
		return ItemInventory[Index];
}

int32 UInventoryComponent::FindItem(FString String)
{
	for (int32 Index = 0; Index < ItemInventory.Num(); Index++)
	{
		if (ItemInventory[Index]->ItemName == String)
			return Index;
	}
	return -1;
}

void UInventoryComponent::initializeInventory()
{
	for (int32 Index = ItemInventory.Num(); Index < InventorySize; Index++)
	{
		UItemObject* NoneItem = NewObject<UItemObject>(this);
		NoneItem->InventorySlotNumber = Index;
		NoneItem->World = GetWorld();
		NoneItem->Inventory = this;
		ItemInventory.Add(NoneItem);
	}
}

void UInventoryComponent::SetBlankInventory(int32 Index)
{
	if (Index == -1)
		return;
	UItemObject* NoneItem = NewObject<UItemObject>(this);
	NoneItem->InventorySlotNumber = Index;
	NoneItem->World = GetWorld();
	NoneItem->Inventory = this;
	ItemInventory[Index] = NoneItem;
}

int32 UInventoryComponent::GetInventorySlotIndex()
{
	for (int32 Index = 0; Index < ItemInventory.Num(); Index++)
	{
		if (ItemInventory[Index]->ItemEnum == EItemEnum::E_None)
		{
			return Index;
		}
	}
	return -1;
}

void UInventoryComponent::SwapItem(int32 IndexA, int32 IndexB)
{
	if (IndexA == -1 || IndexB == -1)
		return;
	int32 TempIndex = ItemInventory[IndexA]->InventorySlotNumber;
	ItemInventory[IndexA]->InventorySlotNumber = ItemInventory[IndexB]->InventorySlotNumber;
	ItemInventory[IndexB]->InventorySlotNumber = TempIndex;
	ItemInventory.Swap(IndexA, IndexB);
	OnInventoryUpdated.Broadcast();
}