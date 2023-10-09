// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Character))
	{
		SetLocomotionState();
	}
}

void UPlayerAnimInstance::SetLocomotionState()
{
	if (IsFalling)
		return;

	FVector Acceleration = CharacterMovement->GetCurrentAcceleration();
	FVector VelocityNormalize = Velocity;
	Acceleration.Normalize(0.0001f);
	VelocityNormalize.Normalize(0.0001f);
	if (FVector::DotProduct(Acceleration, VelocityNormalize) < -0.5f)
		CharacterLocoState = ECharacterState::E_Idle;
	else
	{
		if (IsMovementWithinThresholds(1.0f, 300.0f, 0.5f))
		{
			CharacterLocoState = ECharacterState::E_Jog;
		}
		else if (IsMovementWithinThresholds(1.0f, 0.0f, 0.01f))
		{
			CharacterLocoState = ECharacterState::E_Jog;
		}
		else
		{
			CharacterLocoState = ECharacterState::E_Idle;
		}
	}
}
