// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "EquipItemObject.generated.h"


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
		bool IsEquip = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FString EquipDescription = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FAddItemInfo EquipItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 EquipIndex;

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