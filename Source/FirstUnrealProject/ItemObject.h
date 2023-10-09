// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoolTimeInterface.h"
#include "ItemObject.generated.h"

UENUM(BlueprintType)
enum class EItemRank : uint8
{
	E_Rank_None UMETA(DisplayName = "None"),
	E_Rank_Normal UMETA(DisplayName = "Normal"),
	E_Rank_Magic UMETA(DisplayName = "Magic"),
	E_Rank_Rare UMETA(DisplayName = "Rare"),
	E_Rank_Epic UMETA(DisplayName = "Epic"),
	E_Rank_Unique UMETA(DisplayName = "Unique"),
	E_Rank_Legendary UMETA(DisplayName = "Legendary"),
	E_Rank_Mythic UMETA(DisplayName = "Mythic")
};

UENUM(BlueprintType)
enum class EItemEnum : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Item_Consumable UMETA(DisplayName = "Consumable"),
	E_Item_QuestItem UMETA(DisplayName = "QuestItem"),
	E_Item_Miscellaneous UMETA(DisplayName = "Miscellaneous"),
	E_Equip_Helmet UMETA(DisplayName = "Helmet"),
	E_Equip_Weapons UMETA(DisplayName = "Weapons"),
	E_Equip_TopArmor UMETA(DisplayName = "TopArmor"),
	E_Equip_BottomArmor UMETA(DisplayName = "BottomArmor"),
	E_Equip_Boots UMETA(DisplayName = "Boots"),
	E_Equip_Gloves UMETA(DisplayName = "Gloves"),
	E_Equip_Ring UMETA(DisplayName = "Ring")
};

USTRUCT(BlueprintType)
struct FAddItemInfo
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

	FAddItemInfo operator+(const FAddItemInfo& Other) const
	{
		FAddItemInfo Result;
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
	FAddItemInfo operator-(const FAddItemInfo& Other) const
	{
		FAddItemInfo Result;
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
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class FIRSTUNREALPROJECT_API UItemObject : public UObject, public ICoolTimeInterface
{
	GENERATED_BODY()
public:
	UItemObject();
public:

	UPROPERTY(Transient)
		class UWorld* World;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		FString UseActionText = "";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UStaticMesh* StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UTexture2D* Texture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
		FString ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
		FString ItemDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 0.0))
		int32 Count = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 0.0))
		float Weight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
		float DropChance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time")
		float CoolTime = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time")
		float CurrentCoolTime = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time")
		bool IsUse = false;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
		int32 QuickSlotNumber = -1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EItemRank ItemRank = EItemRank::E_Rank_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		EItemEnum ItemEnum = EItemEnum::E_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
		class UInventoryComponent* Inventory;


	virtual void StartCooldown_Implementation(float CooldownTime);
	virtual void EndCooldown_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Item")
		bool OnUse(class ACustomCharacter* Character);
	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual bool OnUse_Implementation(class ACustomCharacter* Character);
};
