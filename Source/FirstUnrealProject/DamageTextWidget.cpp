// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextWidget.h"
#include "Components/TextBlock.h"
void UDamageTextWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UDamageTextWidget::SetDamageValue(int32 Damage) const
{
	DamageTextBox->SetText(FText::AsNumber(Damage));
	//UE_LOG(LogTemp, Log, TEXT("Interface"));
	return false;
}
