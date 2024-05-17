// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Character/CustomCharacter.h"
#include "ItemObject.h"

#include "EnemyInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UEnemyInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
public:
	UEnemyInventoryComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<class UItemObject*> DropItemInventory;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void AddDropItem(UItemObject* Item);
	void DropItem();

	void SetEnemyEquipItem(ECustomCharacterState EnemyState);
	void EquipItem(EItemEnum ItemEnum);
	void UnEquipItem(EItemEnum ItemEnum);
};
