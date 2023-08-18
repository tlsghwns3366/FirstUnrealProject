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
	EquipDescription = "\n";
	if (EquipItemState.AddDamage != 0)
		EquipDescription += FString::Printf(TEXT("Damage + %f\n"), EquipItemState.AddDamage);
	if (EquipItemState.AddShild != 0)
		EquipDescription += FString::Printf(TEXT("Shild + %f\n"), EquipItemState.AddShild);
	if (EquipItemState.AddHP != 0)
		EquipDescription += FString::Printf(TEXT("HP + %f\n"), EquipItemState.AddHP);
	if (EquipItemState.AddStamina != 0)
		EquipDescription += FString::Printf(TEXT("HP + %f\n"), EquipItemState.AddStamina);
	if (EquipItemState.AddCriticalChance != 0)
		EquipDescription += FString::Printf(TEXT("CriticalChance + %f\n"), EquipItemState.AddCriticalChance);
	if (EquipItemState.AddCriticalDamage != 0)
		EquipDescription += FString::Printf(TEXT("CriticalDamage + %f\n"), EquipItemState.AddCriticalDamage);
	if (EquipItemState.AddDodgeChance != 0)
		EquipDescription += FString::Printf(TEXT("DodgeChance + %f\n"), EquipItemState.AddDodgeChance);
}
