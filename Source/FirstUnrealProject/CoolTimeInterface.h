// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CoolTimeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCoolTimeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTUNREALPROJECT_API ICoolTimeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartCooldown(float CooldownTime);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartBuffCooldown(float CooldownTime);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void EndCooldown();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void EndBuffCooldown();
};
