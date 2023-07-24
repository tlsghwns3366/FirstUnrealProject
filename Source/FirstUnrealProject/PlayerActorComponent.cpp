// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActorComponent.h"

// Sets default values for this component's properties
UPlayerActorComponent::UPlayerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	Level = 1;
}


void UPlayerActorComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevel(Level);
}

// Called when the game starts
void UPlayerActorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UPlayerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerActorComponent::SetLevel(int32 _Level)
{
	Level = _Level;
}

void UPlayerActorComponent::OnDamaged(float DamageAmount)
{
	OnhpUpdated.Broadcast();
}

