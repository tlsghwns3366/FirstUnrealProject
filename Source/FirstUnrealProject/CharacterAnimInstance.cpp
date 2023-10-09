// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "CustomCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Character = Cast<ACustomCharacter>(Pawn);
		if (IsValid(Character))
		{
			CharacterMovement = Character->GetCharacterMovement();
			SkeletalMeshComponent = Character->GetMesh();
		}
	}
}

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Character))
	{
		//UE_LOG(LogTemp, Log, TEXT("%f"), Speed);
		Velocity = CharacterMovement->Velocity;
		Speed = Velocity.Length();
		InputVector = CharacterMovement->GetLastInputVector().GetClampedToSize(0.0f, 1.0f);
		IsMoving = Speed > 3.0f;
		IsFalling = CharacterMovement->IsFalling();
	}
}

void UCharacterAnimInstance::AnimNotify_Hit()
{
	OnAttackHit.Broadcast();
}

bool UCharacterAnimInstance::IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration)
{
	if (MinCurrentSpeed <= Speed && MinMaxSpeed <= CharacterMovement->GetMaxSpeed() && MinInputAcceleration <= InputVector.Length())
		return true;
	else
		return false;
}
