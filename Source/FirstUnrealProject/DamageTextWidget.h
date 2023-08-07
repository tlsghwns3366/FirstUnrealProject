// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageInterface.h"
#include "DamageTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UDamageTextWidget : public UUserWidget ,public IDamageInterface
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DamageTextBox;
public:
	virtual bool SetDamageValue(int32 Damage) const override;
	virtual void SetTextColor(FColor Color);
};
