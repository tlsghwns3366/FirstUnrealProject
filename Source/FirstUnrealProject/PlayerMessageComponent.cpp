// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMessageComponent.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include "ItemObject.h"

// Sets default values for this component's properties
UPlayerMessageComponent::UPlayerMessageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerMessageComponent::BeginPlay()
{
	Super::BeginPlay();
	InventoryComponent = Cast<APlayerCharacter>(GetOwner())->InventoryComponent;

	/*InventoryComponent->OnInventoryUpdated.AddDynamic(this,&UPlayerMessageComponent::InventoryFind);
	InventoryFind();
	*/
	// ...
}


// Called every frame
void UPlayerMessageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPlayerMessageComponent::AddMessage(FSystemMessage Message)
{
	SystemMessageArray.Add(Message);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlayerMessageComponent::RemoveMessage, 5.f, false);
	MessageUpdated.Broadcast();
	return true;
}

void UPlayerMessageComponent::RemoveMessage()
{
	if (SystemMessageArray.Num() > 0)
	{
		SystemMessageArray.RemoveAt(0);
		MessageUpdated.Broadcast();
	}
}

bool UPlayerMessageComponent::AddQuest(FString String)
{
	if (!CurrentActiveQuest.Contains(String))
	{
		//CurrentActiveQuest.Add(String);
		//CurrentQuest.Add(QuestData);

		//QuestUpdated.Broadcast();
		return true;
	}
	else
		return false;
}

bool UPlayerMessageComponent::CompleteQuest(/*FString String, FQuestData Data*/)
{
	/*
	if (CurrentActiveQuest.Contains(String))
	{
		if(!ClearQuest.Contains(String))
			ClearQuest.Add(String);

		int32 Index;
		CurrentActiveQuest.Find(String, Index);
		CurrentActiveQuest.RemoveAt(Index);
		CurrentQuest.RemoveAt(Index);

		QuestUpdated.Broadcast();
		return true;
	}
	else
	*/
		return false;
}

bool UPlayerMessageComponent::SetQuestInfo(/*FString string, FQuestData Data*/)
{
	/*
	for (int32 Index = 0; Index < Data.ClearQuest.Num(); Index++)
	{
		if (!ClearQuest.Contains(Data.ClearQuest[Index]))
			return false;
	}
	QuestData = Data;
	QuestString = string;
	*/
	return true;
}

bool UPlayerMessageComponent::FindQuest(FString String)
{
	if (ClearQuest.Contains(String))
		return true;
	if (CurrentActiveQuest.Contains(String))
		return true;
	return false;
}

bool UPlayerMessageComponent::FinishQuest(FString String)
{
	/*
	for (int32 index = 0; index < CurrentQuest.Num(); index++)
	{
		if (CurrentQuest[index].QuestNumber == String)
		{
			for (int32 QuestIndex = 0; QuestIndex < CurrentQuest[index].QuestObjective.Num(); QuestIndex++)
			{
				if (!CurrentQuest[index].QuestObjective[QuestIndex].IsCompleted)
					return false;
			}
			return true;
		}
	}
	*/
	return false;
}

void UPlayerMessageComponent::ShowQuestWidget()
{
	WidgetUpdated.Broadcast();
	QuestUpdated.Broadcast();
}

void UPlayerMessageComponent::InventoryFind()
{
	/*
	for (int32 IndexA = 0; IndexA < CurrentQuest.Num(); IndexA++)
	{
		for (int32 IndexB = 0; IndexB < CurrentQuest[IndexA].QuestObjective.Num(); IndexB++)
		{
			if (CurrentQuest[IndexA].QuestObjective[IndexB].QuestType == EQuestType::E_ItemCollect)
			{
				bool Itemfind = false;
				for (UItemObject* Item : InventoryComponent->ItemInventory)
				{
					if (CurrentQuest[IndexA].QuestObjective[IndexB].ObjectiveName.ToString() == Item->ItemName)
					{
						CurrentQuest[IndexA].QuestObjective[IndexB].CurrentItemCollect = Item->Count;
						if (CurrentQuest[IndexA].QuestObjective[IndexB].CurrentItemCollect >= CurrentQuest[IndexA].QuestObjective[IndexB].ObjectiveItemCollect)
							CurrentQuest[IndexA].QuestObjective[IndexB].IsCompleted = true;
						else
							CurrentQuest[IndexA].QuestObjective[IndexB].IsCompleted = false;
						Itemfind = true;
					}
				}
				if (!Itemfind)
					CurrentQuest[IndexA].QuestObjective[IndexB].CurrentItemCollect = 0;
			}
		}
	}
	*/
}

void UPlayerMessageComponent::EnemyKill(FString String)
{
	/*
	for (int32 IndexA = 0; IndexA < CurrentQuest.Num(); IndexA++)
	{
		for (int32 IndexB = 0; IndexB < CurrentQuest[IndexA].QuestObjective.Num(); IndexB++)
		{
			if (CurrentQuest[IndexA].QuestObjective[IndexB].QuestType == EQuestType::E_EnemyKill)
			{
				if (CurrentQuest[IndexA].QuestObjective[IndexB].CurrentKill < CurrentQuest[IndexA].QuestObjective[IndexB].ObjectiveKill)
				{
					if (CurrentQuest[IndexA].QuestObjective[IndexB].ObjectiveName.ToString() == String)
					{
						CurrentQuest[IndexA].QuestObjective[IndexB].CurrentKill++;
						if (CurrentQuest[IndexA].QuestObjective[IndexB].CurrentKill >= CurrentQuest[IndexA].QuestObjective[IndexB].ObjectiveKill)
							CurrentQuest[IndexA].QuestObjective[IndexB].IsCompleted = true;
						QuestUpdated.Broadcast();
					}
				}
			}
		}
	}*/
}
