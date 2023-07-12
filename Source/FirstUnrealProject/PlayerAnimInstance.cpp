// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Player = Cast<APlayerCharacter>(Pawn);
		if (IsValid(Player))
		{
			CharacterMovement = Player->GetCharacterMovement();
			MaxWalkSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
		}
	}
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Player))
	{
		//UE_LOG(LogTemp, Log, TEXT("%f"), Speed);
		Velocity = CharacterMovement->Velocity;
		if (IsRun)
		{
			Speed = Velocity.Size() * 1.3; 
			Player->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed * 1.3;
		}
		else
		{
			Speed = Velocity.Size() * 1;
			Player->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		}

		IsMoving = Speed > 3.0f;
		IsFalling = CharacterMovement->IsFalling();
	}
}

void UPlayerAnimInstance::SetIsRun(float value)
{
	if (value == 0)
		IsRun = false;
	else
		IsRun = true;
}
