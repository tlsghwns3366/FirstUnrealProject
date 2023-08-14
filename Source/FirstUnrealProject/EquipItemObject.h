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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		EItemEnum ItemEnum {
		EItemEnum::E_None
	};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		bool Equip = false;
	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void OnUse_Implementation(class ACustomCharacter* Character) override;
};