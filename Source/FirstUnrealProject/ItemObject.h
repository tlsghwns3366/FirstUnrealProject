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
		FText UseActionText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UStaticMesh* StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UTexture2D* Texture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		FText ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
		FText ItemDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
		int32 Count;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
		float Weight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
		float DropChance;

	UPROPERTY()
		class UInventoryComponent* Inventory;

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class APlayerCharacter* Player) ;
};
