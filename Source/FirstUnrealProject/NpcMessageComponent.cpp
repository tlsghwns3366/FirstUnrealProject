// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcMessageComponent.h"
#include "NpcCharacter.h"
#include "PlayerMessageComponent.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UNpcMessageComponent::UNpcMessageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UNpcMessageComponent::BeginPlay()
{
	Super::BeginPlay();
	SetDataTable();
	// ...
	
}


// Called every frame
void UNpcMessageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNpcMessageComponent::SetDataTable()
{/*
	if (IsValid(TalkTable))
	{
		NpcInfo = *TalkTable->FindRow<FNpcInfo>(Cast<ANpcCharacter>(GetOwner())->NpcId, FString(""));
	}
	*/
}

FSystemMessage UNpcMessageComponent::GetNpcMessage(int32 Index)
{
	FSystemMessage Message;
	/*
	int32 MaxIndex = NpcInfo.NpcDescription.Num();
	
	Message.SystemName = NpcInfo.NpcName;
	if (Index < MaxIndex)
	{
		Message.SystemMessage = NpcInfo.NpcDescription[Index];
		return Message;
	}
	*/
	return Message;
}

FSystemMessage UNpcMessageComponent::GetQuestMessage(int32 QuestIndex, int32 DescriptionIndex)
{
	FSystemMessage Message;
	/*
	int32 MaxIndex = QuestInfo[QuestIndex].QuestNpcDescription.Num();
	
	Message.SystemName = NpcInfo.NpcName;
	if (DescriptionIndex < MaxIndex)
	{
		Message.SystemMessage = QuestInfo[QuestIndex].QuestNpcDescription[DescriptionIndex];
		return Message;
	}
	*/
	return Message;
}

int32 UNpcMessageComponent::FindQuest(APlayerCharacter* Player)
{   /*
	if (Player != nullptr)
	{
		for (int32 Index = 0; Index < NpcInfo.QuestList.Num(); Index++)
		{
			if (!Player->PlayerMessageComponent->FindQuest(NpcInfo.QuestList[Index]))
			{
				if(Player->PlayerMessageComponent->SetQuestInfo(NpcInfo.QuestList[Index],QuestInfo[Index]))
					return Index;
			}
			else
			{
				if (Player->PlayerMessageComponent->FinishQuest(QuestInfo[Index].QuestNumber))
				{
					Player->PlayerMessageComponent->CompleteQuest(NpcInfo.QuestList[Index], QuestInfo[Index]);
				}
			}
		}
	}*/
	return -1.f;
}