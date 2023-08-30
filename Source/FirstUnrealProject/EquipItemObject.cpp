// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemObject.h"
#include "CharacterStateComponent.h"

UEquipItemObject::UEquipItemObject()
{
}

void UEquipItemObject::OnUse_Implementation(ACustomCharacter* Character)
{

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
