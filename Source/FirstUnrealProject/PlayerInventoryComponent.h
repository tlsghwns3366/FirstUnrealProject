// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIRSTUNREALPROJECT_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
public:
	UPlayerInventoryComponent();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UEquipItemObject*> EquipInventory;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	bool EquipItem(class UEquipItemObject* Item);
	bool UnEquipItem(UEquipItemObject* Item)
};
