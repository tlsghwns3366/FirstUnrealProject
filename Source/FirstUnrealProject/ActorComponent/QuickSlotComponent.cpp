// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotComponent.h"
#include "Item/EquipItemObject.h"
#include "Item/ConsumableItemObject.h"
#include "Character/CustomCharacter.h"
// Sets default values for this component's properties
UQuickSlotComponent::UQuickSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();
	QuickSlot.SetNum(10);
	// ...
}


// Called every frame
void UQuickSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UQuickSlotComponent::AddObject(int32 Num, UObject* Object)
{
	if (Object != nullptr)
	{
		int32 FindIndex = QuickSlot.Find(Object);
		QuickSlot[Num] = Object;
		if (UItemObject* ItemObject = Cast<UItemObject>(Object))
		{
			ItemObject->QuickSlotNumber = Num;
		}
		if (FindIndex != -1)
		{
			RemoveObject(FindIndex);
		}
		QuickSlotUpdste.Broadcast();
		return true;
	}
	return false;
}

void UQuickSlotComponent::RemoveObject(int32 Num)
{
	if (UItemObject* ItemObject = Cast<UItemObject>(QuickSlot[Num]))
	{
		ItemObject->QuickSlotNumber = -1;
	}
	QuickSlot[Num] = nullptr;
	QuickSlotUpdste.Broadcast();
}

void UQuickSlotComponent::UseSlot(int32 Num)
{
	Num = Num - 1.f;
	if (QuickSlot[Num] != nullptr)
	{
		if (UEquipItemObject* EquipItem = Cast<UEquipItemObject>(QuickSlot[Num]))
		{
			EquipItem->OnUse_Implementation(Cast<ACustomCharacter>(GetOwner()));
		}
		if (UConsumableItemObject* ConsumbleItem = Cast<UConsumableItemObject>(QuickSlot[Num]))
		{
			ConsumbleItem->OnUse_Implementation(Cast<ACustomCharacter>(GetOwner()));
		}
	}
}
