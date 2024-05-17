// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon_Bow.h"

AWeapon_Bow::AWeapon_Bow()
{

}

void AWeapon_Bow::WeaponInitialize_Implementation(UEquipItemObject* Item)
{
}

void AWeapon_Bow::SetStringPosition_Implementation(bool IsBool, FVector Location)
{
	IsPull = IsBool;
	StringLocation = Location;
}