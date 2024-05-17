// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

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

void UDamageTextWidget::SetTextColor(FColor Color)
{
	FLinearColor TextColor = FLinearColor(Color);
	DamageTextBox->SetColorAndOpacity(TextColor);
}

void UDamageTextWidget::SetRandomPosition()
{
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(DamageTextBox->Slot);
	float X = FMath::FRandRange(-50.f, 50.f);
	float Y = FMath::FRandRange(-20.f, 20.f);
	if (PanelSlot)
	{
		PanelSlot->SetPosition(FVector2D(X, Y));
	}
}
