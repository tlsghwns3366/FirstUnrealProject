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
			PlayerMaxWalkSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
			PlayerRunSpeed = 1.3f;
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
			Speed = Velocity.Size() * PlayerRunSpeed;
			Player->GetCharacterMovement()->MaxWalkSpeed = PlayerMaxWalkSpeed * PlayerRunSpeed;
		}
		else
		{
			Speed = Velocity.Size() * 1;
			Player->GetCharacterMovement()->MaxWalkSpeed = PlayerMaxWalkSpeed;
		}
		IsMoving = Speed > 3.0f;
		IsFalling = CharacterMovement->IsFalling();
	}
}