// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "PlayerMessageComponent.h"
#include "CharacterStateComponent.h"

#include "Character/PlayerCharacter.h"

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
		if (QuestData != nullptr)
		{
			QuestInfo.Add(*QuestData);
		}
	}
}

FQuestData* UQuestComponent::GetQuestData(FString string)
{
	for (int32 Index = 0; Index < QuestInfo.Num(); Index++)
	{
		if (string == QuestInfo[Index].QuestString)
		{
			return &QuestInfo[Index];
		}
	}
	return nullptr;
}

bool UQuestComponent::QuestCheck(FQuestData* QuestData, AActor* Caller)
{
	if (QuestData == nullptr)
		return false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(Caller);
	if (Player->MainStateComponent->GetFinalState().Level < QuestData->MinLv)
		return false;
	if (Player->PlayerMessageComponent->CurrentCheck(QuestData->QuestString))
		return false;
	if (Player->PlayerMessageComponent->ClearCheck(QuestData->QuestString) && !QuestData->IsLoop)
		return false;

	for (int32 Index = 0; Index < QuestData->ClearQuest.Num(); Index++)
	{
		if (!Player->PlayerMessageComponent->ClearCheck(QuestData->ClearQuest[Index]))
			return false;
	}

	return true;
}

bool UQuestComponent::ObjectiveCheck(FQuestData* CurrentQuest)
{
	if (CurrentQuest == nullptr)
		return false;
	for (int32 Index = 0; Index < CurrentQuest->QuestObjective.Num(); Index++)
	{
		if (!CurrentQuest->QuestObjective[Index].IsCompleted)
			return false;
	}
	return true;
}