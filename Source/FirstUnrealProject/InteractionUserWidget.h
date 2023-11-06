// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UInteractionUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* NameTextBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* InteractionTextBox;
public:
	void SetWidget();
	void SetWidget(class UItemObject* Object);
};
