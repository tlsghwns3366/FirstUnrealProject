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
			IsRun = false;
			CharacterMovement = Enemy->GetCharacterMovement();
			EnemyMaxWalkSpeed = Enemy->GetCharacterMovement()->MaxWalkSpeed;
			EnemyRunSpeed = 200.f;
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
		if (IsRun)
		{
			Speed = Velocity.Size() + EnemyRunSpeed;
			Enemy->GetCharacterMovement()->MaxWalkSpeed = EnemyMaxWalkSpeed + EnemyRunSpeed;
		}
		else
		{
			Speed = Velocity.Size();
			Enemy->GetCharacterMovement()->MaxWalkSpeed = EnemyMaxWalkSpeed;
		}
		IsMoving = Speed > 3.0f;
		IsFalling = CharacterMovement->IsFalling();

		LeftRight = this->CalculateDirection(Enemy->GetVelocity(), Enemy->GetActorRotation());
	}
}

void UEnemyAnimInstance::SetIsRun(bool Run)
{
	IsRun = Run;
}
