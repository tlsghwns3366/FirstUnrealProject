// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcMessageComponent.h"
#include "PlayerMessageComponent.h"

#include "Character/NpcCharacter.h"
#include "Character/PlayerCharacter.h"

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
	// ...
	
}


// Called every frame
void UNpcMessageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNpcMessageComponent::SetDataTable()
{	
}

FSystemMessage UNpcMessageComponent::GetNpcMessage(int32 Index)
{
	FSystemMessage Message;
	return Message;
}

FSystemMessage UNpcMessageComponent::GetQuestMessage(int32 QuestIndex, int32 DescriptionIndex)
{
	FSystemMessage Message;
	return Message;
}

int32 UNpcMessageComponent::FindQuest(APlayerCharacter* Player)
{   
	return -1.f;
}