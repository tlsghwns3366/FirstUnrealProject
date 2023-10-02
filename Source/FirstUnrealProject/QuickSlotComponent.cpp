// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotComponent.h"
#include "EquipItemObject.h"
#include "ConsumableItemObject.h"
#include "CustomCharacter.h"

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
	int32 FindIndex = QuickSlot.Find(Object);
	if (FindIndex == -1)
	{
		QuickSlot[Num] = Object;
		QuickSlotUpdste.Broadcast();
		return true;
	}
	else
	{
		RemoveObject(QuickSlot.Find(Object));
		QuickSlot[Num] = Object;
		QuickSlotUpdste.Broadcast();
		return true;
	}
}

void UQuickSlotComponent::RemoveObject(int32 Num)
{
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
