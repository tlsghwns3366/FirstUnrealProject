// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemObject.h"
#include "CustomCharacter.h"
#include "CharacterStateComponent.h"
#include "InventoryComponent.h"
#include "CoolDownComponent.h"
UEquipItemObject::UEquipItemObject()
{
}

bool UEquipItemObject::OnUse_Implementation(ACustomCharacter* Character)
{
	if (IsUse || Inventory == nullptr)
		return false;
	else
	{
		ICoolTimeInterface* Interface = Cast<ICoolTimeInterface>(this);
		if (Interface)
		{
			Interface->Execute_StartCooldown(this,CoolTime);
		}
		IsUse = true;
		if (IsValid(Character))
		{
			MainStateComponent = Character->MainStateComponent;
			Character->CoolDownComponent->AddCoolDownObject(this);
		}
		if (!Equip)
		{
			EquipItem(this);
		}
		else
		{
			UnEquipItem(this);
		}
		return true;
	}

}

void UEquipItemObject::SetDescription()
{
	EquipDescription = "";
	if (EquipItemState.AddDamage != 0)
		EquipDescription += FString::Printf(TEXT("Damage + %.1f\n"), EquipItemState.AddDamage);
	if (EquipItemState.AddShild != 0)
		EquipDescription += FString::Printf(TEXT("Shild + %.1f\n"), EquipItemState.AddShild);
	if (EquipItemState.AddHP != 0)
		EquipDescription += FString::Printf(TEXT("HP + %.1f\n"), EquipItemState.AddHP);
	if (EquipItemState.AddMP != 0)
		EquipDescription += FString::Printf(TEXT("MP + %.1f\n"), EquipItemState.AddMP);
	if (EquipItemState.AddStamina != 0)
		EquipDescription += FString::Printf(TEXT("Stamina + %.1f\n"), EquipItemState.AddStamina);
	if (EquipItemState.AddCriticalChance != 0)
		EquipDescription += FString::Printf(TEXT("CriticalChance + %.1f\n"), EquipItemState.AddCriticalChance);
	if (EquipItemState.AddCriticalDamage != 0)
		EquipDescription += FString::Printf(TEXT("CriticalDamage + %.1f\n"), EquipItemState.AddCriticalDamage);
	if (EquipItemState.AddDodgeChance != 0)
		EquipDescription += FString::Printf(TEXT("DodgeChance + %.1f\n"), EquipItemState.AddDodgeChance);
}

bool UEquipItemObject::EquipItem(UEquipItemObject* Item)
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
		Item->Inventory->ItemInventory.RemoveSingle(Item);
		Item->Inventory->EquipInventory.Add(Item);
		Item->Inventory->OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}

bool UEquipItemObject::UnEquipItem(UEquipItemObject* Item)
{
	if (Item->Equip)
	{
		MainStateComponent->SetEquip(nullptr, Item->ItemEnum);
		Item->Equip = false;
		Item->Inventory->EquipInventory.RemoveSingle(Item);
		Item->Inventory->ItemInventory.Add(Item);
		Item->Inventory->OnInventoryUpdated.Broadcast();
		return true;
	}
	else
		return false;
}
