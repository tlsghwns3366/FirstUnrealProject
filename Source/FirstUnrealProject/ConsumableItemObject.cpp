// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemObject.h"
#include "CoolDownComponent.h"
#include "CustomCharacter.h"
#include "CharacterStateComponent.h"
#include "InventoryComponent.h"
#include "PlayerCharacter.h"
#include "QuickSlotComponent.h"

void UConsumableItemObject::SetDescription()
{
	ConsumableItemDescription = "";
	if (ConsumableItemInfo.AddDamage != 0)
		ConsumableItemDescription += FString::Printf(TEXT("Damage + %.1f\n"), ConsumableItemInfo.AddDamage);
	if (ConsumableItemInfo.AddShild != 0)
		ConsumableItemDescription += FString::Printf(TEXT("Shild + %.1f\n"), ConsumableItemInfo.AddShild);
	if (ConsumableItemInfo.AddHP != 0)
		ConsumableItemDescription += FString::Printf(TEXT("HP + %.1f\n"), ConsumableItemInfo.AddHP);
	if (ConsumableItemInfo.AddMP != 0)
		ConsumableItemDescription += FString::Printf(TEXT("MP + %.1f\n"), ConsumableItemInfo.AddMP);
	if (ConsumableItemInfo.AddStamina != 0)
		ConsumableItemDescription += FString::Printf(TEXT("Stamina + %.1f\n"), ConsumableItemInfo.AddStamina);
	if (ConsumableItemInfo.AddCriticalChance != 0)
		ConsumableItemDescription += FString::Printf(TEXT("CriticalChance + %.1f\n"), ConsumableItemInfo.AddCriticalChance);
	if (ConsumableItemInfo.AddCriticalDamage != 0)
		ConsumableItemDescription += FString::Printf(TEXT("CriticalDamage + %.1f\n"), ConsumableItemInfo.AddCriticalDamage);
	if (ConsumableItemInfo.AddDodgeChance != 0)
		ConsumableItemDescription += FString::Printf(TEXT("DodgeChance + %.1f\n"), ConsumableItemInfo.AddDodgeChance);
}

bool UConsumableItemObject::OnUse_Implementation(ACustomCharacter* Character)
{
	if (IsUse || Inventory == nullptr)
		return false;
	if (IsValid(Character))
	{
		MainCharacter = Character;
		Character->CoolDownComponent->AddCoolDownObject(this);
		Character->MainStateComponent->CharacterAddState(this);
		ICoolTimeInterface* Interface = Cast<ICoolTimeInterface>(this);
		if (Interface)
		{
			Interface->Execute_StartCooldown(this, CoolTime);
			Interface->Execute_StartBuffCooldown(this, CoolTime);
		}
		IsUse = true;
		Count = Count - 1.0f;
		Inventory->OnInventoryUpdated.Broadcast();
		if (Count <= 0.f)
		{
			Inventory->RemoveItem(this);
			ConditionalBeginDestroy();
		}
		return true;
	}
	return false;
}

void UConsumableItemObject::StartBuffCooldown_Implementation(float CooldownTime)
{
	IsDuration = true;
	CurrentBuffDuration = Duration;
}

void UConsumableItemObject::EndBuffCooldown_Implementation()
{
	IsDuration = false;
	MainCharacter->MainStateComponent->CharacterRemoveState(this);
}