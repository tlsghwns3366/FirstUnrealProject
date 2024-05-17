// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon.h"
#include "WeaponInterface.h"
#include "Weapon_Bow.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API AWeapon_Bow : public AWeapon, public IWeaponInterface
{
	GENERATED_BODY()
public:
	AWeapon_Bow();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BowData")
	bool IsPull;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BowData")
	FVector StringLocation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<class AWeapon> ArrowBlueprint;


public:
	virtual void WeaponInitialize_Implementation(UEquipItemObject* Item);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void SetStringPosition(bool IsBool, FVector Location);
	virtual void SetStringPosition_Implementation(bool IsBool, FVector Location);
};
