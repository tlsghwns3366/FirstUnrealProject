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
		Velocity = CharacterMovement->Velocity;
		if (IsRun)
		{
			Speed = Velocity.Size() * 2;
			UE_LOG(LogTemp, Log, TEXT("TRUE"));
		}
		else
		{
			Speed = Velocity.Size() * 1;
			UE_LOG(LogTemp, Log, TEXT("FALSE"));
		}
		UE_LOG(LogTemp, Log, TEXT("%f"), Speed);
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
