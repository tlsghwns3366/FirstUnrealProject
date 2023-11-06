// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionUserWidget.h"
#include "ItemObject.h"
#include "Components/TextBlock.h"

void UInteractionUserWidget::SetWidget(UItemObject* Object)
{
	if (Object != nullptr)
	{
		NameTextBox->SetText(FText::FromString(Object->ItemName));
		FText TextToSet = FText::FromString(TEXT(" 획득"));
		InteractionTextBox->SetText(TextToSet);
		FLinearColor TextColor;
		switch (Object->ItemRank)
		{
		case EItemRank::E_Rank_Normal:
			TextColor = FLinearColor(FColor(255, 255, 255, 255));
			break;
		case EItemRank::E_Rank_Magic:
			TextColor = FLinearColor(FColor(50, 205, 50, 255));
			break;
		case EItemRank::E_Rank_Rare:
			TextColor = FLinearColor(FColor(30, 144, 255, 255));
			break;
		case EItemRank::E_Rank_Epic:
			TextColor = FLinearColor(FColor(128, 00, 128, 255));
			break;
		case EItemRank::E_Rank_Unique:
			TextColor = FLinearColor(FColor(255, 215, 0, 255));
			break;
		case EItemRank::E_Rank_Legendary:
			TextColor = FLinearColor(FColor(255, 69, 0, 255));
			break;
		case EItemRank::E_Rank_Mythic:
			TextColor = FLinearColor(FColor(128, 0, 0, 255));
			break;
		}
		NameTextBox->SetColorAndOpacity(TextColor);
	}
}

void UInteractionUserWidget::SetWidget()
{
	FText TextToSet = FText::FromString(TEXT("대화"));
	InteractionTextBox->SetText(TextToSet);
}
