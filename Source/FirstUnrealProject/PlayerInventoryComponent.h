// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIRSTUNREALPROJECT_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
public:
	UPlayerInventoryComponent();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D LastInventoryPosition;

	UPROPERTY(BlueprintAssignable)
		FOnInventoryUpdated OnInventoryUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool AddItem(class UItemObject* item);
	bool RemoveItem(class UItemObject* item);
};
