// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateComponent.h"
#include "Weapon.h"
#include "EquipItemObject.h"
#include "CustomCharacter.h"
#include "ConsumableItemObject.h"
#include "Logging/StructuredLog.h"

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
	SetAddState();
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
	if (CurrentStamina < FinalState.MaxStamina)
		StaminaRegen(DeltaTime);
	TickAggroCount(DeltaTime);
	// ...
}

void UCharacterStateComponent::SetState()
{
	float HpPer = CurrentHp / FinalState.MaxHp;
	float MpPer = CurrentMp / FinalState.MaxMp;
	float StaminaPer = CurrentStamina / FinalState.MaxStamina;

	FinalState = CharacterState + CharacterInfo;

	CurrentHp = HpPer * FinalState.MaxHp;
	CurrentMp = MpPer * FinalState.MaxMp;
	CurrentStamina = StaminaPer * FinalState.MaxStamina;

	OnHpMpUpdated.Broadcast();
	OnStaminaUpdated.Broadcast();
}

void UCharacterStateComponent::SetAddState()
{
	FAddItemInfo TempInfo;
	if (Helmat != nullptr)
		TempInfo = TempInfo + Helmat->EquipItemInfo;
	if (TopArmor != nullptr)
		TempInfo = TempInfo + TopArmor->EquipItemInfo;
	if (BottomArmor != nullptr)
		TempInfo = TempInfo + BottomArmor->EquipItemInfo;
	if (Boots != nullptr)
		TempInfo = TempInfo + Boots->EquipItemInfo;
	if (Gloves != nullptr)
		TempInfo = TempInfo + Gloves->EquipItemInfo;
	if (Weapons_1 != nullptr)
		TempInfo = TempInfo + Weapons_1->EquipItemInfo;
	if (Weapons_2 != nullptr)
		TempInfo = TempInfo + Weapons_2->EquipItemInfo;
	if (Ring_1 != nullptr)
		TempInfo = TempInfo + Ring_1->EquipItemInfo;
	if (Ring_2 != nullptr)
		TempInfo = TempInfo + Ring_2->EquipItemInfo;

	for (auto* Object : CharacterAddStateInfo)
	{
		if (UConsumableItemObject* ConsumableItemObject = Cast<UConsumableItemObject>(Object))
		{
			TempInfo = TempInfo + ConsumableItemObject->ConsumableItemInfo;
		}
	}
	CharacterInfo = TempInfo;
	SetState();
}

FCharacterState UCharacterStateComponent::GetFinalState()
{
	return FinalState;
}

bool UCharacterStateComponent::CharacterAddState(UObject* Object)
{
	if (Object != nullptr)
	{
		//Duplicate check
		if (UConsumableItemObject* ConsumableItemObject = Cast<UConsumableItemObject>(Object))
		{
			int32 BuffCount = CharacterAddStateInfo.Num();
			for (int32 i = 0; i < BuffCount; i++)
			{
				if (ConsumableItemObject->ItemName == Cast<UConsumableItemObject>(CharacterAddStateInfo[i])->ItemName)
				{
					CharacterRemoveState(CharacterAddStateInfo[i]);
				}
			}
		}
		CharacterAddStateInfo.Add(Object);
		SetAddState();
		OnStateUpdated.Broadcast();
		OnHpMpUpdated.Broadcast();
		return true;
	}
	else
	{
		return false;
	}
}

bool UCharacterStateComponent::CharacterRemoveState(UObject* Object)
{
	if (Object != nullptr)
	{
		CharacterAddStateInfo.RemoveSingle(Object);
		SetAddState();
		OnStateUpdated.Broadcast();
		OnHpMpUpdated.Broadcast();
		return true;
	}
	else
	{
		return false;
	}
}



bool UCharacterStateComponent::UseHp(float NewHp)
{
	CurrentHp = CurrentHp - NewHp;
	if (CurrentHp <= 0.f)
	{
		IsDie = true;
		CurrentHp = 0.f;
		OnHpbarUpdated.Broadcast();
		OnHpMpUpdated.Broadcast();
		return false;
	}
	else
	{
		OnHpMpUpdated.Broadcast();
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


float UCharacterStateComponent::GetPhysicalDamage()
{
	return FMath::FRandRange(FinalState.AttackDamage * 0.85, FinalState.AttackDamage * 1.15);
}

void UCharacterStateComponent::SetEquip(UEquipItemObject* Item, EItemEnum ItemEnum)
{
	switch (ItemEnum)
	{
	case EItemEnum::E_Equip_Helmet:
		if (Item->IsEquip)
		{
			if (Helmat != nullptr)
				Helmat->UnEquipItem(Helmat);
			Helmat = Item;
			//Spawn
		}
		else
		{
			Helmat = nullptr;
		}
		break;
	case EItemEnum::E_Equip_Weapons:
		if (Item->IsEquip)
		{
			if (Weapons_1 != nullptr)
				Weapons_1->UnEquipItem(Weapons_1);
			Weapons_1 = Item;
			EquipItemSpawn(Weapons_1);
		}
		else
		{
			Weapons_1 = nullptr;
			if (AttachedWeapon != nullptr)
				AttachedWeapon->Destroy();
		}
		break;
	case EItemEnum::E_Equip_TopArmor:
		if (Item->IsEquip)
		{
			if (TopArmor != nullptr)
				TopArmor->UnEquipItem(Helmat);
			TopArmor = Item;
			//Spawn
		}
		else
		{
			TopArmor = nullptr;
		}
		break;
	case EItemEnum::E_Equip_BottomArmor:
		if (Item->IsEquip)
		{
			if (BottomArmor != nullptr)
				BottomArmor->UnEquipItem(Helmat);
			BottomArmor = Item;
			//Spawn
		}
		else
		{
			BottomArmor = nullptr;
		}
		break;
	case EItemEnum::E_Equip_Boots:
		if (Item->IsEquip)
		{
			if (Boots != nullptr)
				Boots->UnEquipItem(Helmat);
			Boots = Item;
			//Spawn
		}
		else
		{
			Boots = nullptr;
		}
		Boots = Item;
		break;
	case EItemEnum::E_Equip_Gloves:
		if (Item->IsEquip)
		{
			if (Gloves != nullptr)
				Gloves->UnEquipItem(Helmat);
			Gloves = Item;
			//Spawn
		}
		else
		{
			Gloves = nullptr;
		}
		Gloves = Item;
		break;
	case EItemEnum::E_Equip_Ring:
		if (Item->IsEquip)
		{
			if (Ring_1 != nullptr && Ring_2 == nullptr)
			{
				Ring_2 = Item;
				Ring_2->EquipIndex = 1;
			}
			else
			{
				if (Item->EquipIndex == 0)
				{
					if (Ring_1 != nullptr)
						Ring_1->UnEquipItem(Ring_1);
					Ring_1 = Item;
					Ring_1->EquipIndex = 0;
				}
				else
				{
					if (Ring_2 != nullptr)
						Ring_2->UnEquipItem(Ring_2);
					Ring_2 = Item;
					Ring_2->EquipIndex = 1;
				}
			}
		}
		else
		{
			if (Item->EquipIndex == 0)
			{
				Ring_1 = nullptr;
			}
			else if (Item->EquipIndex == 1)
			{
				Ring_2 = nullptr;
				Item->EquipIndex = 0;
			}
		}
		break;
	default:
		break;
	}
	SetAddState();
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

void UCharacterStateComponent::TickAggroCount(float DeltaTime)
{
	if (IsAggro)
	{
		if (AggroCount <= 100.f)
		{
			if (AggroT || AggroTAdd)
			{
				float Alpha = AggroCount / 100.f;
				AggroCurrentTime = AggroTime * powf(Alpha, 1.f / AggroExp);
				AggroT = false;
				AggroTAdd = false;
			}
			float Ratio = FMath::InterpEaseIn(0.f, 1.0f, FMath::Clamp(AggroCurrentTime / AggroTime, 0.f, 1.0f), AggroExp);
			AggroCount = FMath::Lerp(0.f, 100.f, Ratio);
			AggroCurrentTime = FMath::Clamp(AggroCurrentTime + DeltaTime * AggroSensitivity, 0.f, AggroTime);
			AggroCurrentDelayTime = AggroDelayTime;
		}
	}
	else
	{
		if (AggroCount >= 0 && AggroCurrentDelayTime <= 0)
		{
			if (!AggroT || AggroTAdd)
			{
				float Alpha = AggroCount / 100.f;
				AggroCurrentTime = AggroTime * (1.f - powf(1.f - Alpha, 1.f / AggroExp));
				AggroT = true;
				AggroTAdd = false;
			}
			float Ratio = FMath::InterpEaseOut(0.f, 1.0f, FMath::Clamp(AggroCurrentTime / AggroTime, 0.f, 1.0f), AggroExp);
			AggroCount = FMath::Lerp(0.f, 100.f, Ratio);
			AggroCurrentTime = FMath::Clamp(AggroCurrentTime - DeltaTime, 0.f, AggroTime);
		}
		else
		{
			AggroCurrentDelayTime = FMath::Clamp(AggroCurrentDelayTime - DeltaTime, 0.f, AggroDelayTime);
		}
	}
}

void UCharacterStateComponent::AddAggro(float Value)
{
	AggroCurrentDelayTime = AggroDelayTime;
	AggroCount += Value * AggroSensitivity;
	AggroTAdd = true;
	if (AggroCount > 100.f)
		AggroCount = 100.f;
}
