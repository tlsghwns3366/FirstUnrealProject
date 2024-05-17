// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Character/EnemyCharacter.h"
#include "Controller/EnemyAIController.h"
#include "ActorComponent/CharacterStateComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (IsValid(Character))
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
		if (!EnemyController)
			EnemyController = Cast<AEnemyAIController>(Character->GetController());
		else
		{
			InputVector = EnemyController->GetInputVector();
		}
	}
}