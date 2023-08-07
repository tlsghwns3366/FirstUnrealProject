// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageType_Critical.h"
#include "DamageComponent.h"

bool UDamageType_Critical::SetAttackType(UDamageComponent* DamageComponent, float Damage) const
{
	DamageComponent->OnCritical(Damage);
	return false;
}
