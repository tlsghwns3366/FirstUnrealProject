// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipItemObject.h"
#include "WeaponEquipItemObject.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UWeaponEquipItemObject : public UEquipItemObject
{
	GENERATED_BODY()
public:
	UWeaponEquipItemObject();
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform")
		FTransform StaticMeshTransform;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform")
		FTransform StartPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform")
		FTransform EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FName AttachSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		TArray<UAnimMontage*> CharacterAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float StaminaCost;
};
