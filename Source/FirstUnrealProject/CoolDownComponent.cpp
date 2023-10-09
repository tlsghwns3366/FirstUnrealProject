// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolDownComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ItemObject.h"
#include "ConsumableItemObject.h"

// Sets default values for this component's properties
UCoolDownComponent::UCoolDownComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCoolDownComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UCoolDownComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CoolDownTimeObject.Num() > 0)
	{
		for (int32 i = 0; i < CoolDownTimeObject.Num(); i++)
		{
			TickCoolTime(CoolDownTimeObject[i], DeltaTime);
		}
	}
	if (CoolDownTimeConsumableItem.Num() > 0)
	{
		for (int32 i = 0; i < CoolDownTimeConsumableItem.Num(); i++)
		{
			TickCoolTimeConsumbleItem(CoolDownTimeConsumableItem[i], DeltaTime);
		}
	}
	// ...
}

void UCoolDownComponent::AddCoolDownObject(UObject* Object)
{
	if (Object != nullptr)
		CoolDownTimeObject.Add(Object);
	if (UConsumableItemObject* ConsumableItem = Cast<UConsumableItemObject>(Object))
	{
		//Duplicate check
		int32 BuffCount= CoolDownTimeConsumableItem.Num();
		for (int32 i = 0; i < BuffCount; i++)
		{
			if (CoolDownTimeConsumableItem[i]->ItemName == ConsumableItem->ItemName)
			{
				RemoveBuffCoolDownObject(CoolDownTimeConsumableItem[i]);
			}
		}
		CoolDownTimeConsumableItem.Add(ConsumableItem);
	}
}

void UCoolDownComponent::RemoveCoolDownObject(UObject* Object)
{
	if (Object != nullptr)
		CoolDownTimeObject.RemoveSingle(Object);
}

void UCoolDownComponent::RemoveBuffCoolDownObject(UConsumableItemObject* Object)
{
	if (Object != nullptr)
		CoolDownTimeConsumableItem.RemoveSingle(Object);
}

void UCoolDownComponent::TickCoolTime(UObject* Object, float DeltaTime)
{
	if (UItemObject* Item = Cast<UItemObject>(Object))
	{
		if (Item->IsUse)
		{
			float CurrentTime = Item->CurrentCoolTime;
			Item->CurrentCoolTime = UKismetMathLibrary::FInterpTo(CurrentTime, CurrentTime - 1.0f, DeltaTime, 1.0f);
			if (Item->CurrentCoolTime <= 0.f)
			{
				ICoolTimeInterface* Interface = Cast<ICoolTimeInterface>(Item);
				if (Interface)
				{
					Interface->Execute_EndCooldown(Item);
				}
				RemoveCoolDownObject(Item);
			}
		}
	}
}

void UCoolDownComponent::TickCoolTimeConsumbleItem(UConsumableItemObject* Object, float DeltaTime)
{
	if (Object != nullptr)
	{
		if (Object->IsDuration)
		{
			float CurrentTime = Object->CurrentBuffDuration;
			Object->CurrentBuffDuration = UKismetMathLibrary::FInterpTo(CurrentTime, CurrentTime - 1.0f, DeltaTime, 1.0f);
			if (Object->CurrentBuffDuration <= 0.f)
			{
				ICoolTimeInterface* Interface = Cast<ICoolTimeInterface>(Object);
				if (Interface)
				{
					Interface->Execute_EndBuffCooldown(Object);
				}
				RemoveBuffCoolDownObject(Object);
			}
		}
	}
}
