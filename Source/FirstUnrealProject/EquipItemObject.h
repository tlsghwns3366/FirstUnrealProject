// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "EquipItemObject.generated.h"

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
		float AddMP = 0.f;
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
		Result.AddMP = AddMP + Other.AddMP;
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
		Result.AddMP = AddMP - Other.AddMP;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FString EquipDescription = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FEquipItemInfo EquipItemState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		class UCharacterStateComponent* MainStateComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		 TSubclassOf<class AWeapon> WeaponBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FName AttachSocket;
public:
		virtual bool OnUse_Implementation(class ACustomCharacter* Character) override;
	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void SetDescription();

		virtual bool EquipItem(class UEquipItemObject* Item);
		virtual bool UnEquipItem(UEquipItemObject* Item);

};