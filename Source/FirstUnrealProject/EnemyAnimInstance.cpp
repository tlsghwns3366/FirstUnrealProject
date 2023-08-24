// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if(IsValid(Character))
		EnemyController = Cast<AEnemyAIController>(Character->GetController());
}

void UEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)	
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Character))
	{
		LeftRight = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
		if (EnemyController != nullptr)
		{
			IsNight = EnemyController->GetBlackboardComponent()->GetValueAsBool(FName("IsNight"));
			FindTarget = EnemyController->GetBlackboardComponent()->GetValueAsBool(FName("FindTarget"));
		}
		else
		{
			EnemyController = Cast<AEnemyAIController>(Character->GetController());
		}
	}
}