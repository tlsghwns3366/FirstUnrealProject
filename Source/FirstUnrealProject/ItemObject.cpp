// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

UItemObject::UItemObject()
{

}

void UItemObject::StartCooldown_Implementation(float CooldownTime)
{
	CurrentCoolTime = CooldownTime;
}

void UItemObject::EndCooldown_Implementation()
{
	IsUse = false;
}

void UItemObject::OnUse_Implementation(ACustomCharacter* Character)
{
}
