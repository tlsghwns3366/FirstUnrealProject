// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMessageComponent.h"

#include "Character/PlayerCharacter.h"
#include "Item/InventoryComponent.h"
#include "Item/ItemObject.h"
#include "Character/NpcCharacter.h"

// Sets default values for this component's properties
UPlayerMessageComponent::UPlayerMessageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	LastSelectQuest = -1;
	// ...
}


// Called when the game starts
void UPlayerMessageComponent::BeginPlay()
{
	Super::BeginPlay();
	InventoryComponent = Cast<APlayerCharacter>(GetOwner())->InventoryComponent;
	InventoryComponent->OnInventoryUpdated.AddDynamic(this,&UPlayerMessageComponent::InventoryCheck);
	InventoryCheck();
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

void UPlayerMessageComponent::RemoveMenu()
{
	PlayerSelect.Empty();
	MenuUpdated.Broadcast();
}

bool UPlayerMessageComponent::AddQuest(FQuestData* Data)
{
	if (Data != nullptr)
	{
		CurrentQuest.Add(*Data);
		InventoryCheck();
		QuestUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UPlayerMessageComponent::RemoveQuest(FQuestData* Data)
{
	if (Data != nullptr)
	{
		for (int32 Index = 0; Index < CurrentQuest.Num(); Index++)
		{
			if (*Data->QuestString == CurrentQuest[Index].QuestString)
			{
				if (LastSelectQuest == Index)
					LastSelectQuest = -1;
				ClearQuest.AddUnique(Data->QuestString);
				CurrentQuest.RemoveAt(Index);
				QuestUpdated.Broadcast();
				return true;
			}
		}
	}
		return false;
}
bool UPlayerMessageComponent::QuestClear(FQuestData* Data)
{
	//Reward 
	if (Data != nullptr)
	{
		for (int32 Index = 0; Index < CurrentQuest.Num(); Index++)
		{
			if (*Data->QuestString == CurrentQuest[Index].QuestString)
			{
				for (int32 ObjectiveIndex = 0; ObjectiveIndex < CurrentQuest[Index].QuestObjective.Num(); ObjectiveIndex++)
				{
					if (CurrentQuest[Index].QuestObjective[ObjectiveIndex].QuestType == EQuestType::E_ItemCollect)
					{
						int32 ItemIndex = InventoryComponent->FindItem(CurrentQuest[Index].QuestObjective[ObjectiveIndex].ObjectiveString);
						InventoryComponent->RemoveItem(InventoryComponent->ItemInventory[ItemIndex]);
					}
				}
				RemoveQuest(Data);
				return true;
			}
		}
	}
	return false;
}
void UPlayerMessageComponent::SetNpcMenuInfo(TArray<struct FPlayerSelect> *Menu)
{
	PlayerSelect.Empty();
	PlayerSelectNumber = 0;
	MenuUpdated.Broadcast();
	if (Menu != nullptr)
	{
		PlayerSelect = *Menu;
	}
	MenuUpdated.Broadcast();
}

void UPlayerMessageComponent::NextSelectNumber()
{
	PlayerSelectNumber++;
	if (PlayerSelect.Num() <= PlayerSelectNumber)
		PlayerSelectNumber = 0;
	MenuUpdated.Broadcast();
}

void UPlayerMessageComponent::SetIsTalk(bool NpcTalk)
{
	IsTalk = NpcTalk;
}

bool UPlayerMessageComponent::CurrentCheck(FString String)
{
	for (int32 Index = 0; Index < CurrentQuest.Num(); Index++)
	{
		if (CurrentQuest[Index].QuestString == String)
			return true;
	}
	return false;
}

bool UPlayerMessageComponent::ClearCheck(FString String)
{
	if (ClearQuest.Contains(String))
		return true;
	return false;
}

FQuestData* UPlayerMessageComponent::GetCurrentQuest(FString String)
{
	for (int32 Index = 0; Index < CurrentQuest.Num(); Index++)
	{
		if (CurrentQuest[Index].QuestString == String)
			return &CurrentQuest[Index];
	}
	return nullptr;
}

void UPlayerMessageComponent::EnemyKillCount(FString String)
{
	for (int32 Index = 0; Index < CurrentQuest.Num(); Index++)
	{
		for (int32 Index2 = 0; Index2 < CurrentQuest[Index].QuestObjective.Num(); Index2++)
		{
			if (CurrentQuest[Index].QuestObjective[Index2].ObjectiveString == String)
			{
				if (CurrentQuest[Index].QuestObjective[Index2].ObjectiveKill > CurrentQuest[Index].QuestObjective[Index2].CurrentKill)
				{
					CurrentQuest[Index].QuestObjective[Index2].CurrentKill++;
					if (CurrentQuest[Index].QuestObjective[Index2].ObjectiveKill == CurrentQuest[Index].QuestObjective[Index2].CurrentKill)
						CurrentQuest[Index].QuestObjective[Index2].IsCompleted = true;
					QuestUpdated.Broadcast();
				}
			}
		}
	}
}

void UPlayerMessageComponent::InventoryCheck()
{
	for (int32 Index = 0; Index < CurrentQuest.Num(); Index++)
	{
		for (int32 Index2 = 0; Index2 < CurrentQuest[Index].QuestObjective.Num(); Index2++)
		{
			if (CurrentQuest[Index].QuestObjective[Index2].QuestType == EQuestType::E_ItemCollect)
			{
				int32 ItemIndex = InventoryComponent->FindItem(CurrentQuest[Index].QuestObjective[Index2].ObjectiveString);
				UItemObject* Item = InventoryComponent->GetIndexItem(ItemIndex);
				if (Item != nullptr)
				{
					if (Item->ItemName == CurrentQuest[Index].QuestObjective[Index2].ObjectiveString)
					{
						CurrentQuest[Index].QuestObjective[Index2].CurrentItemCollect = Item->Count;
						if (CurrentQuest[Index].QuestObjective[Index2].CurrentItemCollect >= CurrentQuest[Index].QuestObjective[Index2].ObjectiveItemCollect)
							CurrentQuest[Index].QuestObjective[Index2].IsCompleted = true;
						else
							CurrentQuest[Index].QuestObjective[Index2].IsCompleted = false;
						QuestUpdated.Broadcast();
					}
				}
			}
		}
	}
}
