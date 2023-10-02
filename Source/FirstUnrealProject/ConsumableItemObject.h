// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "ConsumableItemObject.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UConsumableItemObject : public UItemObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FString ConsumableItemDescription = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		FAddItemInfo ConsumableItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		class ACustomCharacter* MainCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		bool IsDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		float Duration = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		float CurrentBuffDuration = 0.0f;

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void SetDescription();

	virtual bool OnUse_Implementation(class ACustomCharacter* Character) override;
	virtual void StartBuffCooldown_Implementation(float CooldownTime) override;
	virtual void EndBuffCooldown_Implementation() override;
	
};
