// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateComponent.h"

// Sets default values for this component's properties
UCharacterStateComponent::UCharacterStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterStateComponent::SetLevel(int32 _Level)
{
	Level = _Level;
	Hp = MaxHp;
}


void UCharacterStateComponent::SetHp(float NewHp)
{
	Hp = NewHp;
	if (Hp <= 0.f)
	{
		IsDie = true;
		Hp = 0.f;
	}
}

float UCharacterStateComponent::GetPhysicalDamage()
{
	return FMath::FRandRange(AttackDamage * 0.85, AttackDamage * 1.15);
}
UEquipItemObject* UCharacterStateComponent::GetEquip(UEquipItemObject* Item)
{
	UEquipItemObject* EquippedItem = nullptr;
	switch (Item->ItemEnum)
	{
	case EItemEnum::E_Equip_Helmet:
		EquippedItem = Helmat;
		break;
	case EItemEnum::E_Equip_Weapons:
		EquippedItem = Weapons_1;
		break;
	case EItemEnum::E_Equip_TopArmor:
		EquippedItem = TopArmor;
		break;
	case EItemEnum::E_Equip_BottomArmor:
		EquippedItem = BottomArmor;
		break;
	case EItemEnum::E_Equip_Boots:
		EquippedItem = Boots;
		break;
	case EItemEnum::E_Equip_Gloves:
		EquippedItem = Gloves;
		break;
	case EItemEnum::E_Equip_Ring_1:
		EquippedItem = Ring_1;
		break;
	case EItemEnum::E_Equip_Ring_2:
		EquippedItem = Ring_2;
		break;
	}
	return EquippedItem;
}

bool UCharacterStateComponent::SetEquip(UEquipItemObject* Item)
{
	switch (Item->ItemEnum)
	{
	case EItemEnum::E_Equip_Helmet:
		Helmat = Item;
		return true;
		break;
	case EItemEnum::E_Equip_Weapons:
		Weapons_1 = Item;
		return true;
		break;
	case EItemEnum::E_Equip_TopArmor:
		TopArmor = Item;
		return true;
		break;
	case EItemEnum::E_Equip_BottomArmor:
		BottomArmor = Item;
		return true;
		break;
	case EItemEnum::E_Equip_Boots:
		Boots = Item;
		return true;
		break;
	case EItemEnum::E_Equip_Gloves:
		Gloves = Item;
		return true;
		break;
	case EItemEnum::E_Equip_Ring_1:
		Ring_1 = Item;
		return true;
		break;
	case EItemEnum::E_Equip_Ring_2:
		Ring_2 = Item;
		return true;
		break;
	default:
		break;
	}
	return false;
}