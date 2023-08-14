// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryComponent.h"
#include "EquipItemObject.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InventorySize = 20; //Default 20
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}