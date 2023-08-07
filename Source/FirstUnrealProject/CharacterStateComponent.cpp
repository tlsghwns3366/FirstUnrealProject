// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateComponent.h"

// Sets default values for this component's properties
UCharacterStateComponent::UCharacterStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterStateComponent::SetLevel(int32 _Level)
{
	Level = _Level;
	Hp = MaxHp;
}


void UCharacterStateComponent::SetHp(float NewHp)
{
	Hp = NewHp;
	if (Hp <= 0.f)
	{
		IsDie = true;
		Hp = 0.f;
	}
}

float UCharacterStateComponent::GetPhysicalDamage()
{
	return FMath::FRandRange(AttackDamage * 0.85, AttackDamage * 1.15);
}
