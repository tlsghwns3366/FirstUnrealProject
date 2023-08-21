// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipItemObject.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 InventorySize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UItemObject*> ItemInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UItemObject*> DefaultInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UEquipItemObject*> EquipInventory;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCharacterStateComponent* MainStateComponent;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D LastInventoryPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D LastEquipmentPosition;

	UPROPERTY(BlueprintAssignable)
		FOnInventoryUpdated OnInventoryUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UFUNCTION(BlueprintCallable)
	virtual bool AddItem(class UItemObject* Item);
	UFUNCTION(BlueprintCallable)
	virtual bool RemoveItem(class UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	virtual bool EquipItem(class UEquipItemObject* Item);
	UFUNCTION(BlueprintCallable)
	virtual bool UnEquipItem(UEquipItemObject* Item);
};
