// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "EquipItemObject.generated.h"

UENUM(BlueprintType)
enum class EItemEnum : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Equip_Helmet UMETA(DisplayName = "Helmet"),
	E_Equip_Weapons UMETA(DisplayName = "Weapons"),
	E_Equip_TopArmor UMETA(DisplayName = "TopArmor"),
	E_Equip_BottomArmor UMETA(DisplayName = "BottomArmor"),
	E_Equip_Boots UMETA(DisplayName = "Boots"),
	E_Equip_Gloves UMETA(DisplayName = "Gloves"),
	E_Equip_Ring_1 UMETA(DisplayName = "Ring_1"),
	E_Equip_Ring_2 UMETA(DisplayName = "Ring_2"),
};
USTRUCT(BlueprintType)
struct FEquipItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AddDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AddShild = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AddHP = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AddStamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AddCriticalChance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AddCriticalDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AddDodgeChance = 0.f;



	FEquipItemInfo operator+(const FEquipItemInfo& Other) const
	{
		FEquipItemInfo Result;
		Result.AddDamage = AddDamage + Other.AddDamage;
		Result.AddShild = AddShild + Other.AddShild;
		Result.AddHP = AddHP + Other.AddHP;
		Result.AddStamina = AddStamina + Other.AddStamina;
		Result.AddCriticalChance = AddCriticalChance + Other.AddCriticalChance;
		Result.AddCriticalDamage = AddCriticalDamage + Other.AddCriticalDamage;
		Result.AddDodgeChance = AddDodgeChance + Other.AddDodgeChance;
		return Result;
	}
	FEquipItemInfo operator-(const FEquipItemInfo& Other) const
	{
		FEquipItemInfo Result;
		Result.AddDamage = AddDamage - Other.AddDamage;
		Result.AddShild = AddShild - Other.AddShild;
		Result.AddHP = AddHP - Other.AddHP;
		Result.AddStamina = AddStamina - Other.AddStamina;
		Result.AddCriticalChance = AddCriticalChance - Other.AddCriticalChance;
		Result.AddCriticalDamage = AddCriticalDamage - Other.AddCriticalDamage;
		Result.AddDodgeChance = AddDodgeChance - Other.AddDodgeChance;
		return Result;
	}
};

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UEquipItemObject : public UItemObject
{
	GENERATED_BODY()
public:
	UEquipItemObject();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		bool Equip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EItemEnum ItemEnum = EItemEnum::E_None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FString EquipDescription = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FEquipItemInfo EquipItemState;
public:
	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void OnUse_Implementation(class ACustomCharacter* Character) override;
	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void SetDescription();
};