// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageType_Physical.h"
#include "DamageComponent.h"

bool UDamageType_Physical::SetAttackType(UDamageComponent* DamageComponent, float Damage) const
{
	DamageComponent->OnPhysical(Damage);
	return false;
}
