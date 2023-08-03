// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStateActorComponent.h"
#include "EnemyCharacter.h"
#include "MainGameState.h"
#include "EnemyAnimInstance.h"

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
	Enemy = Cast<AEnemyCharacter>(GetOwner());
	float GameSpeed = MainState->GameSpeed;
	if (RestTime < 200.f && !Enemy->Anim->GetRest())
		RestTime += FMath::Lerp(0, 200.f, 0.001f * GameSpeed);
	if (Enemy->Anim->GetRest())
	{
		if (RestTime > 0)
			RestTime -= FMath::Lerp(0, 200.f, 0.01f * GameSpeed);
		else
		{
			Enemy->Anim->SetRest(false);
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("%f"),RestTime);
}
