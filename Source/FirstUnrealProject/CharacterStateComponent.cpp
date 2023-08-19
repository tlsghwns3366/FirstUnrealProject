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
	SetEquipState();
	SetState();
	CurrentHp = FinalState.MaxHp;
	CurrentStamina = FinalState.MaxStamina;
	CurrentSpeed = FinalState.WalkSpeed;
	OnhpUpdated.Broadcast();
	OnExpUpdated.Broadcast();
	// ...
	
}


// Called every frame
void UCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterStateComponent::SetState()
{
	FinalState = CharacterState + CharacterEquipItemState;
}

void UCharacterStateComponent::SetHp(float NewHp)
{
	CurrentHp = NewHp;
	if (CurrentHp <= 0.f)
	{
		IsDie = true;
		CurrentHp = 0.f;
	}
}

float UCharacterStateComponent::GetPhysicalDamage()
{
	return FMath::FRandRange(FinalState.AttackDamage * 0.85, FinalState.AttackDamage * 1.15);
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
		SetEquipState();
		SetState();
		return true;
		break;
	case EItemEnum::E_Equip_Weapons:
		Weapons_1 = Item;
		SetEquipState();
		SetState();
		return true;
		break;
	case EItemEnum::E_Equip_TopArmor:
		TopArmor = Item;
		SetEquipState();
		SetState();
		return true;
		break;
	case EItemEnum::E_Equip_BottomArmor:
		BottomArmor = Item;
		SetEquipState();
		SetState();
		return true;
		break;
	case EItemEnum::E_Equip_Boots:
		Boots = Item;
		SetEquipState();
		SetState();
		return true;
		break;
	case EItemEnum::E_Equip_Gloves:
		Gloves = Item;
		SetEquipState();
		SetState();
		return true;
		break;
	case EItemEnum::E_Equip_Ring_1:
		Ring_1 = Item;
		SetEquipState();
		SetState();
		return true;
		break;
	case EItemEnum::E_Equip_Ring_2:
		Ring_2 = Item;
		SetEquipState();
		SetState();
		return true;
		break;
	default:
		break;
	}
	return false;
}

void UCharacterStateComponent::SetEquipState()
{
	FEquipItemInfo TempInfo;
	if (Helmat != nullptr)
		TempInfo = TempInfo + Helmat->EquipItemState;
	if (TopArmor != nullptr)
		TempInfo = TempInfo + TopArmor->EquipItemState;
	if (BottomArmor != nullptr)
		TempInfo = TempInfo + BottomArmor->EquipItemState;
	if (Boots != nullptr)
		TempInfo = TempInfo + Boots->EquipItemState;
	if (Gloves != nullptr)
		TempInfo = TempInfo + Gloves->EquipItemState;
	if (Weapons_1 != nullptr)
		TempInfo = TempInfo + Weapons_1->EquipItemState;
	if (Weapons_2 != nullptr)
		TempInfo = TempInfo + Weapons_2->EquipItemState;
	if (Ring_1 != nullptr)
		TempInfo = TempInfo + Ring_1->EquipItemState;
	if (Ring_2 != nullptr)
		TempInfo = TempInfo + Ring_2->EquipItemState;
	CharacterEquipItemState = TempInfo;
}
