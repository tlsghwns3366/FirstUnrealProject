// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMessageComponent.h"
#include "PlayerCharacter.h"

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
	// ...
}


// Called every frame
void UPlayerMessageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPlayerMessageComponent::AddMessage(FNpcMessage Message)
{
	NpcMessageArray.Add(Message);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlayerMessageComponent::RemoveMessage, 5.f, false);
	MessageUpdated.Broadcast();
	return true;
}

void UPlayerMessageComponent::RemoveMessage()
{
	if (NpcMessageArray.Num() > 0)
	{
		NpcMessageArray.RemoveAt(0);
		MessageUpdated.Broadcast();
	}
}

bool UPlayerMessageComponent::AddQuest(FString String)
{
	CurrentActiveQuest.Add(String);
	return false;
}

bool UPlayerMessageComponent::CompleteQuest(FString String)
{
	ClearQuest.Add(String);
	return true;
}

bool UPlayerMessageComponent::FindQuest(FString String)
{
	/*
	if (ClearQuest.Find(String) != -1)
		return true;
	if (CurrentActiveQuest.Find(String) != -1)
		return true;*/
	/*
	for (int32 Index = 0; Index < ClearQuest.Num(); Index++)
	{
		if (ClearQuest[Index]==String)
			return true;
	}
	for (int32 Index = 0; Index < CurrentActiveQuest.Num(); Index++)
	{
		if (CurrentActiveQuest[Index].Equals(String))
			return true;
	}*/
	return false;
}
