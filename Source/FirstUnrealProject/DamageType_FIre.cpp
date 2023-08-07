// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageType_FIre.h"
#include "DamageComponent.h"

bool UDamageType_FIre::SetAttackType(UDamageComponent* DamageComponent,float Damage) const
{
	DamageComponent->OnFire(Damage);
	return false;
}
