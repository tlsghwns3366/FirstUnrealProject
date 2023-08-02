// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStateActorComponent.h"
#include "MainGameState.h"

UEnemyStateActorComponent::UEnemyStateActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyStateActorComponent::BeginPlay()
{
	Super::BeginPlay();
	MainState = Cast<AMainGameState>(GetWorld()->GetGameState());
}

void UEnemyStateActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LastTime = CurrentTime;
	CurrentTime = MainState->Hours;
	if (LastTime != CurrentTime)
	{
		if(SleepTime<100.f)
		SleepTime += 5.0f;
	}
}
