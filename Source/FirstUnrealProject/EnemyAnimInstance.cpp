// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Enemy = Cast<AEnemyCharacter>(Pawn);
		if (IsValid(Enemy))
		{
			CharacterMovement = Enemy->GetCharacterMovement();
		}
	}
}

void UEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)	
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Enemy))
	{
		//UE_LOG(LogTemp, Log, TEXT("%f"), Speed);
		Velocity = CharacterMovement->Velocity;
		Speed = Velocity.Size();
		IsMoving = Speed > 3.0f;
		IsFalling = CharacterMovement->IsFalling();

		LeftRight = CalculateDirection(Enemy->GetVelocity(), Enemy->GetActorRotation());
	}
}

void UEnemyAnimInstance::AnimNotify_Hit()
{
	OnAttackHit.Broadcast();
}
