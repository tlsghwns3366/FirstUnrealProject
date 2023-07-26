// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
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


public:
	virtual bool AddItem(UItemObject* item) override;
};
