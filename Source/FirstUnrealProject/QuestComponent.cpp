// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "PlayerCharacter.h"
#include "PlayerMessageComponent.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::SetQuestDataTable(FString string)
{
	if (IsValid(QuestTable))
	{
		FQuestData* QuestData = QuestTable->FindRow<FQuestData>(FName(string), FString(""));
		if(QuestData != nullptr)
			QuestInfo.Add(*QuestData);
	}
}

int32 UQuestComponent::FindQuest(FString string)
{
	for (int32 Index = 0; Index < QuestInfo.Num(); Index++)
	{
		if (string == QuestInfo[Index].QuestString)
		{
			return Index;
		}
	}
	return -1;
}

bool UQuestComponent::QuestCheck(int32 QuestIndex, FString QuestString, AActor* Caller)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Caller);
	for (int32 Index = 0; Index < QuestInfo[QuestIndex].ClearQuest.Num(); Index++)
	{
		if (Player->PlayerMessageComponent->FindQuest(QuestInfo[QuestIndex].ClearQuest[Index]))
			return false;
	}
	return true;
}

void UQuestComponent::ObjectiveCheck(int32 Index)
{

}

FString UQuestComponent::GetQuestTalk(int32 QuestIndex, int32* TalkIndex)
{
	if (*TalkIndex == -1)
	{
		return FString();
	}
	FString QuestString = QuestInfo[QuestIndex].QuestTalkDescription[*TalkIndex].TalkDescription;
	*TalkIndex = QuestInfo[QuestIndex].QuestTalkDescription[*TalkIndex].NextTalk;

	return QuestString;
}
