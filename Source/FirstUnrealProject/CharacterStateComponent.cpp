// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateComponent.h"
#include "Weapon.h"
#include "EquipItemObject.h"
#include "CustomCharacter.h"

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
	CurrentHp = FinalState.MaxHp;
	CurrentMp = FinalState.MaxMp;
	CurrentStamina = FinalState.MaxStamina;
	CurrentSpeed = FinalState.WalkSpeed;
	OnHpMpUpdated.Broadcast();
	OnExpUpdated.Broadcast();
	OnStaminaUpdated.Broadcast();

	// ...
	
}


// Called every frame
void UCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentHp < FinalState.MaxHp || CurrentMp < FinalState.MaxMp)
		HpMpRegen(DeltaTime);
	if(CurrentStamina < FinalState.MaxStamina)
		StaminaRegen(DeltaTime);
	// ...
}

void UCharacterStateComponent::SetState()
{
	FinalState = CharacterState + CharacterEquipItemState;
}

bool UCharacterStateComponent::SetHp(float NewHp)
{
	CurrentHp = NewHp;
	if (CurrentHp <= 0.f)
	{
		IsDie = true;
		CurrentHp = 0.f;
		OnHpbarUpdated.Broadcast();
		return false;
	}
	else
	{
		OnHpbarUpdated.Broadcast();
		return true;
	}
}

bool UCharacterStateComponent::UseStamina(float Amount)
{
	if (CurrentStamina - Amount >= 0)
	{
		CurrentStamina -= Amount;
		OnStaminaUpdated.Broadcast();
		StaminaUseDelay = 1.5f;
		return true;
	}
	else
		return false;
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
	case EItemEnum::E_Equip_Ring:
		EquippedItem = Ring_2;
		break;
	}
	return EquippedItem;
}

bool UCharacterStateComponent::SetEquip(UEquipItemObject* Item, EItemEnum ItemEnum)
{
	switch (ItemEnum)
	{
	case EItemEnum::E_Equip_Helmet:
		Helmat = Item;
		SetEquipState();
		return true;
		break;
	case EItemEnum::E_Equip_Weapons:
		Weapons_1 = Item;
		if (Weapons_1 != nullptr)
		{
			EquipItemSpawn(Weapons_1);
		}
		else
		{
			if (AttachedWeapon != nullptr)
				AttachedWeapon->Destroy();
		}
		SetEquipState();
		return true;
		break;
	case EItemEnum::E_Equip_TopArmor:
		TopArmor = Item;
		SetEquipState();
		return true;
		break;
	case EItemEnum::E_Equip_BottomArmor:
		BottomArmor = Item;
		SetEquipState();
		return true;
		break;
	case EItemEnum::E_Equip_Boots:
		Boots = Item;
		SetEquipState();
		return true;
		break;
	case EItemEnum::E_Equip_Gloves:
		Gloves = Item;
		SetEquipState();
		return true;
		break;
	case EItemEnum::E_Equip_Ring:
		if (Item == nullptr)
		{
			if (Ring_1 != nullptr && Ring_2 == nullptr)
				Ring_1 = Item;
			else
				Ring_2 = Item;
		}
		else
		{
			if (Ring_1 == nullptr)
				Ring_1 = Item;
			else
				Ring_2 = Item;
		}
		SetEquipState();
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
	SetState();
}
void UCharacterStateComponent::HpMpRegen(float DeltaTime)
{
	float TargetHP = FinalState.MaxHp;
	float TargetMP = FinalState.MaxMp;
	if (CurrentHp < TargetHP)
		CurrentHp = FMath::Lerp(CurrentHp, CurrentHp + FinalState.HpRegen, DeltaTime);
	if (CurrentMp < TargetMP)
		CurrentMp = FMath::Lerp(CurrentMp, CurrentMp + FinalState.MpRegen, DeltaTime);
	OnHpMpUpdated.Broadcast();
}
void UCharacterStateComponent::StaminaRegen(float DeltaTime)
{
	float TargetStamina = FinalState.MaxStamina;
	if (StaminaUseDelay >= 0)
		StaminaUseDelay = StaminaUseDelay - DeltaTime;
	if (CurrentStamina < TargetStamina && StaminaUseDelay <= 0)
	{
		CurrentStamina = FMath::Lerp(CurrentStamina, CurrentStamina + FinalState.StaminaRegen, DeltaTime);
	}
	if (CurrentStamina > TargetStamina)
		CurrentStamina = TargetStamina;
	OnStaminaUpdated.Broadcast();
}

void UCharacterStateComponent::EquipItemSpawn(UEquipItemObject* Item)
{
	ACustomCharacter* Character = Cast<ACustomCharacter>(GetOwner());
	switch (Item->ItemEnum)
	{
	case EItemEnum::E_Equip_Helmet:
		break;
	case EItemEnum::E_Equip_Weapons:
	{
		UEquipItemObject* EquipItem = Cast<UEquipItemObject>(Item);
		if (IsValid(EquipItem))
		{
			if (AttachedWeapon != nullptr)
				AttachedWeapon->Destroy();

			FVector ActorLocation = GetOwner()->GetActorLocation();
			FTransform WeaponSocketTransform = Character->GetMesh()->GetSocketTransform(EquipItem->AttachSocket);
			AttachedWeapon = GetWorld()->SpawnActor<AWeapon>(EquipItem->WeaponBlueprint);
			AttachedWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipItem->AttachSocket);
		}
	}
		break;
	case EItemEnum::E_Equip_TopArmor:
		break;
	case EItemEnum::E_Equip_BottomArmor:
		break;
	case EItemEnum::E_Equip_Boots:
		break;
	case EItemEnum::E_Equip_Gloves:
		break;
	default:
		break;
	}
}