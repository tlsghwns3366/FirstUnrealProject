// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class FIRSTUNREALPROJECT_API UItemObject : public UObject
{
	GENERATED_BODY()
public:
	UItemObject();
public:

	UPROPERTY(Transient)
	class UWorld* World;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		FString UseActionText = "";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UStaticMesh* StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UTexture2D* Texture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
		FString ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
		FString ItemDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 0.0))
		int32 Count = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 0.0))
		float Weight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
		float DropChance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
		class UInventoryComponent* Inventory;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
		void OnUse(class ACustomCharacter* Character);
		virtual void OnUse_Implementation(class ACustomCharacter* Character);
};
