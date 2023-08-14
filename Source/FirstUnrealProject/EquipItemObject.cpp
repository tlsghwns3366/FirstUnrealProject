// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemObject.h"
#include "CharacterStateComponent.h"

UEquipItemObject::UEquipItemObject()
{
}

void UEquipItemObject::OnUse_Implementation(ACustomCharacter* Character)
{
	if (Equip)
	{
		Equip = false;
	}
	else
	{
		Equip = true;
	}
}
